#include "../include/Server.hpp"
#include "../include/tools.hpp"
#include "../../Response/Response.hpp"

Server* getServerForClient(maptype& config, int serverId);


void handlingOFCGi(maptype &data, Server *srv, _Cgi *cg, Client *connect){
    
    const int  va = 15020;
    char buffer[va];
    int i = read(cg->fd_in, buffer, va );
    if (i < 0) {
       deleteClient(data, cg->fd_in, cg->fdEp);
        return ;
    }
    
   
    if (i > 0){
        connect->response.append(buffer, i);
        return ;
    }
  
   
    printf("read from cgi {%s} \n", connect->response.c_str());
    srv->respone->applyCgiResponse(connect->response);
    connect->response =   srv->respone->build();

    connect->buildDone = true;
    connect->requestFinish = true;
    deleteClient(data, cg->fd_in, connect->fdEp);
    sendResponse(data, *connect);


}

bool checkTimeout(time_t prevTime, time_t timeout ){
    time_t currentT =  time(NULL);
    if ((currentT - prevTime) >= timeout )
        return true;
    return false;
}

void checkClientsTimeout(maptype& config, int fdEp) 
{
    Client *connect = NULL;
    std::vector<int> ve;
   
    
    for (ConfigIter i = config.begin(); i != config.end(); i++) {
        if (i->second->name == "client") {
            connect = dynamic_cast<Client*>(i->second);

            if (checkTimeout(connect->currentTime, TIMEOUT)) {
                ve.push_back(connect->fd);
            }
        }
        if (i->second->name == "cgi"){
            _Cgi *cg = (_Cgi *) i->second;
            if (checkTimeout(cg->currentTime, TIMEOUT)){
                
                ve.push_back(cg->fd_in);
            }
        }
    }
    for(vector<int>::iterator it = ve.begin(); it != ve.end(); it++   ){

        deleteClient(config, *it, fdEp);
    }
}

void checkClientConnection(maptype &config, Client &connect) {
 
        
    if (connect.sending) {
        connect.buffer = "";
        connect.currentTime = time(NULL);
        setClientSend(connect.fdEp, connect);
    
        return;
    }
    

    // Check if connection should close (from parsed request)

    if (!connect.keepAlive) {
        deleteClient(config, connect.fd, connect.fdEp);
        return;
    }
    
    // Keep-alive: reset for next request
    connect.buildDone = false;
    printf("reset flags to  \n ");
    connect.currentTime = time(NULL);
    connect.requestFinish = false;
    connect.headersOnly = false;
    connect.bodysize = 0;
    connect.byteSent = 0;
    connect.bodySizeStatus = false;
    connect.buffer = "";
    
    connect.parsedRequest = Request();
    
    setClientRead(connect.fdEp, connect);
}
void addCgi(maptype &data, Client &connect , pid_t pip,  int fdIN, int fdOUT){
    _Cgi *obj;

    obj = new _Cgi();
    
    memset(&obj->data, 0, sizeof(obj->data));
    obj->name = "cgi";
    obj->data.events = EPOLLIN  | EPOLLHUP | EPOLLERR;
    obj->data.data.fd =fdIN;
    obj->currentTime = time(NULL);
    obj->fd_in = fdIN;
    obj->pid = pip;
    close (fdOUT);
    obj->fd_client = connect.fd;
    addSockettoEpoll(connect.fdEp, obj->data);
    data[fdIN] = obj;
}

void sendResponse(maptype &config, Client &connect) {
    
    // Get the server configuration
    int n = 1 ;
    int readB = 0;
    char buff[MAXSIZEBYTE];
   
    if (!connect.buildDone){
        Server* srv = getServerForClient(config, connect.serverId);
        srv->respone->processRequest(connect.parsedRequest, *srv);
        connect.response = srv->respone->build();
        connect.buildDone = true;
        if (srv->respone->isCgipending()){
            connect.response.clear();
           
            addCgi(config, connect, srv->respone->getcgiPid(), srv->respone->getcgiReadFd(), srv->respone->getcgiWriteFd() );
            return ;
        }
        if (srv->respone->isLargeFile()){
            printf("file name %s  fd %d \n", srv->respone->getFilePath().c_str(), connect.fd );
            connect.fdFile = open(srv->respone->getFilePath().c_str(), O_RDONLY);
            connect.fdsBuffer.push_back(connect.fdFile);
            connect.fdFile = makeNonBlockingFD(connect.fdFile);
            srv->respone->getFilePath() = "";
        }

    }

    if (!connect.response.empty()){

        printf("response send | %s |\n", connect.response.c_str());
        n = send(connect.fd, connect.response.c_str(), connect.response.size(), 0);
        
        if (n <= 0) {
            deleteClient(config, connect.fd, connect.fdEp);
            return ;
        }
        if (n > 0)
             connect.response.erase(0, n);

    }

    if (connect.response.empty() && connect.fdFile != -1){
        connect.byteRead =  read(connect.fdFile, buff, MAXSIZEBYTE);

        if (connect.byteRead == 0){
            close(connect.fdFile);
            printf("<= 0 read by ");
            connect.fdFile = -1;
        }

        if (connect.byteRead > 0){
            connect.response.append(buff, connect.byteRead);
            setClientSend(connect.fdEp, connect);
            return ;
        }
        
        
    }
    if (connect.response.empty() || connect.fdFile == -1){
        connect.sending  = false ;
        checkClientConnection(config, connect);
        return ;


    }
    connect.sending = true;
    checkClientConnection(config, connect);
}