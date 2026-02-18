#include "../include/Server.hpp"
#include "../include/tools.hpp"
#include "../../Response/Response.hpp"

Server* getServerForClient(maptype& config, int serverId);

// check is if client exists ok 
// check process still contune runing if rading 
///  timeout return bad get way 
// no output what should return like empyt file  string 
// reading 
void handlingOFCGi(maptype &data, int fd, int flag ){
    
    
    char buffer[MAXSIZEBYTE];
    _Cgi *cg = (_Cgi *) returnElement( fd, data);
    if (!cg ) 
        return ;
    Client *connect = (Client *) returnElement(cg->fd_client, data);
    if (!connect){
        kill(cg->pid, SIGTERM);
        deleteClient(data, cg->fd_in, cg->fdEp);
        return ;
    }
    Server *srv = (Server *) returnElement(connect->serverId, data);
        if (!srv)
            return;
    int n = 0;  
    if (flag ==  2){
        if (cg->writeB < connect->parsedRequest.body.size())
         n = write(cg->fdOUT, connect->parsedRequest.body.c_str() + cg->writeB, connect->parsedRequest.body.size() - cg->writeB);
        if (n > 0){
            cg->writeB += n;
        }
        return;
    }
    int status;
    int process = waitpid(cg->pid, &status, WNOHANG );
    if (process == -1){
        flag = -1;
        connect->response = "Status:500 Inter Server Erorr  \r\n\r\n Error ";

    }
    if (flag == 1){

        int i = read(cg->fd_in, buffer, MAXSIZEBYTE );
        if (i < 0 || process < 0 ) {
            kill(cg->pid, SIGTERM);
            printf("invalid data socket or erro in cgi ");
            deleteClient(data, cg->fd_in, cg->fdEp);
            return ;
        }
        
        printf("handling cgi %d %d \n",  i, process);
        if (i > 0 ){
            connect->response.append(buffer, i);
            return ;
      
        }

    }
    if (flag == 0)
        connect->response = "Status:504 Gateway Timeout\r\n\r\ntimeout";
    srv->respone->applyCgiResponse(connect->response);
    connect->response =   srv->respone->build();


    deleteClient(data, cg->fdOUT, connect->fdEp);

    connect->buildDone = true;
    connect->requestFinish = true;
    if (flag == 0 || flag == -1 ){
        kill(cg->pid, SIGTERM);
        waitpid(cg->pid, NULL, WNOHANG );
    }
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
            if (checkTimeout(cg->currentTime, TIMEOUTCGI)){
                ve.push_back(cg->fd_in);
            }
        }
    }
    for(vector<int>::iterator it = ve.begin(); it != ve.end(); it++   ){
      
        if (findElement(config, *it) == "cgi") 
            handlingOFCGi(config, *it, 0);
        else
            deleteClient(config, *it, fdEp);
    }
}

void checkClientConnection(maptype &config, Client &connect) {
 
   
    

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
    
    obj->writeB = 0;
    fdIN = makeNonBlockingFD(fdIN);
    memset(&obj->data, 0, sizeof(obj->data));
    obj->name = "cgi";
    obj->data.events = EPOLLIN  | EPOLLHUP | EPOLLERR;
    obj->data.data.fd = fdIN;
    obj->currentTime = time(NULL);
    obj->fd_in = fdIN;
    obj->pid = pip;
    obj->fdOUT = fdOUT;
    obj->writeB = write(fdOUT, connect.parsedRequest.body.c_str(), connect.parsedRequest.body.size());
    if (obj->writeB != connect.parsedRequest.body.size()){
        obj->OUT.events = EPOLLOUT | EPOLLHUP | EPOLLERR;
        obj->OUT.data.fd = fdOUT;
        addSockettoEpoll(connect.fdEp, obj->OUT);
        data[fdOUT] = obj;
    }
    obj->fd_client = connect.fd;
    addSockettoEpoll(connect.fdEp, obj->data);
    data[fdIN] = obj;
}

void sendResponse(maptype &config, Client &connect) {
    
    // Get the server configuration
    int n = 1 ;
 
    char buff[MAXSIZEBYTE];
   
    if (!connect.buildDone){
        Server* srv = getServerForClient(config, connect.serverId);
        srv->respone->processRequest(connect.parsedRequest, *srv);
        connect.response = srv->respone->build();
        connect.buildDone = true;
        if (srv->respone->isCgipending()){
            connect.response.clear();
           
            addCgi(config, connect, srv->respone->getcgiPid(), srv->respone->getcgiReadFd(), srv->respone->getcgiWriteFd() );
            connect.currentTime = time(NULL); 
            return ;
        }
        if (srv->respone->isLargeFile()){
           
            connect.fdFile = open(srv->respone->getFilePath().c_str(), O_RDONLY);
            connect.fdsBuffer.push_back(connect.fdFile);
            connect.fdFile = makeNonBlockingFD(connect.fdFile);
            srv->respone->getFilePath() = "";
        }

    }

    if (!connect.response.empty()){

        n = send(connect.fd, connect.response.c_str(), connect.response.size(), 0);
        
        if (n <= 0) {
            deleteClient(config, connect.fd, connect.fdEp);
            return ;
        }
        if (n > 0){
            connect.currentTime = time(NULL);  
            connect.response.erase(0, n);
        }

    }

    if (connect.response.empty() && connect.fdFile != -1){
        connect.byteRead =  read(connect.fdFile, buff, MAXSIZEBYTE);

        if (connect.byteRead == 0){
            close(connect.fdFile);
           
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