#include "../include/Server.hpp"
#include "../include/tools.hpp"
#include "../../Response/Response.hpp"

Server* getServerForClient(maptype& config, int serverId);


bool checkTimeout(Client &connect){
    time_t currentT =  time(NULL);
    if ((currentT - connect.prevTime) >= connect.timeout )
        return true;
    return false;
}

void checkClientsTimeout(maptype& config, int fdEp) 
{
    Client *connect = NULL;
    
    for (ConfigIter i = config.begin(); i != config.end(); i++) {
        if (i->second->name == "client") {
            connect = dynamic_cast<Client*>(i->second);
        
            if (checkTimeout(*connect)) {
                
                deleteClient(config, connect->fd, fdEp);
                
            }
        }
    }
}

void checkClientConnection(maptype &config, Client &connect) {
    if (connect.response.empty() && connect.byteRead == 0)
        connect.sending = false;
    if (connect.response.empty() && connect.fdFile == -1)
        connect.sending = false;

        
    if (connect.sending) {
        connect.buffer = "";
        connect.prevTime = time(NULL);
        setClientSend(connect.fdEp, connect);
        cout << "test test ====> " << connect.sending << endl;
        return;
    }
    

    // Check if connection should close (from parsed request)
    if (!connect.keepAlive) {
        deleteClient(config, connect.fd, connect.fdEp);
        return;
    }
    
    // Keep-alive: reset for next request
    connect.buildDone = false;

    connect.prevTime = time(NULL);
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
    close(pip);
    obj->name = "cgi";
    obj->data.events = EPOLLIN  | EPOLLHUP | EPOLLERR;
    obj->data.data.fd = makeNonBlockingFD(fdIN);
    printf("df %d\n", fdIN);
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
        if (srv->respone->isCgipending()){
         
            printf("cgi");
            addCgi(config, connect, srv->respone->getcgiPid(), srv->respone->getcgiReadFd(), srv->respone->getcgiWriteFd() );
            return ;
        }
        connect.buildDone = true;
        if (srv->respone->isLargeFile()){
            printf("file name %s  fd %d \n", srv->respone->getFilePath().c_str(), connect.fd );
            connect.fdFile = open(srv->respone->getFilePath().c_str(), O_RDONLY);
            connect.fdFile = makeNonBlockingFD(connect.fdFile);
            srv->respone->getFilePath() = "";
        }

    }
    int byte = 0;
    while(true){
        if (connect.fdFile != -1){
            byte =  read(connect.fdFile, buff, MAXSIZEBYTE);
            
            if (byte > 0)
                connect.response.append(buff, byte);
        }
        // printf("response | %s |\n", connect.response.c_str());
        n = send(connect.fd, connect.response.c_str(), connect.response.size(), 0);
        if (n < 0  || connect.response.empty())
            break;
        if (n == 0) {
            deleteClient(config, connect.fd, connect.fdEp);
            break;
        }
        if (n > 0)
            connect.response.erase(0, n);

    }

   
    if (connect.response.size() > 0 || connect.byteRead > 0 )
        connect.sending = true;
    
    checkClientConnection(config, connect);
}