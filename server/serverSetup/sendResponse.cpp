#include "../include/Server.hpp"
#include "../include/tools.hpp"
#include "../../Response/Response.hpp"

// check is if client exists ok 
// check process still contune runing if rading 
///  timeout return bad get way 
// no output what should return like empyt file  string 
// reading 

void handlingOfCgi(maptype &data, int fd, int flag, Response &respone ){
    
    
    char buffer[MAXSIZEBYTE] ;
 
    
    _Cgi *cg = (_Cgi *) returnElement( fd, data);
    if (!cg ) 
        return ;

    Client *connect = (Client *)cg->connect;
    if (!connect ){
      
     
        kill(cg->pid, SIGTERM);
        deleteClient(data, cg->fdOUT, cg->fdEp, "", "");
        deleteClient(data, cg->fd_in, cg->fdEp, "" , "");

        return ;
    };

 
    int n = 0;  
    if (flag ==  2){
        if (cg->writeB < (int)connect->parsedRequest.body.size())
         n = write(cg->fdOUT, connect->parsedRequest.body.c_str() + cg->writeB, connect->parsedRequest.body.size() - cg->writeB);
        if (n > 0){
            cg->writeB += n;
        }
        connect->currentTime = time(NULL);
        return;
    }
    int status  = 0;
    int process = waitpid(cg->pid, &status, WNOHANG );
    
    if (WIFEXITED(status) && WEXITSTATUS(status)  != 0)
            process = -1;
    if (process == -1 || flag == 0){
        flag = -1;
        printf("error \n");
        cg->response = "Status:500 Inter Server Error\r\n\r\n Error ";  
    }

    
    if (flag == 1){
        int i = read(cg->fd_in, buffer, MAXSIZEBYTE  );
        if (i < 0 || process < 0 ) {
            kill(cg->pid, SIGTERM);
            deleteClient(data, cg->fdOUT, connect->fdEp, "", "");
            deleteClient(data, cg->fd_in, cg->fdEp,"", connect->ipAddress);
            return ;
        }
        if (i > 0 && *buffer ){
            connect->currentTime = time(NULL);
            cg->response.append(buffer, i);// what happens if cgi came after cgi and cgi other request on the same client  
           
            return ;
        }
    }

    respone.applyCgiResponse(cg->response);   
    if (!connect->sessionCookie.empty()) {
        respone.setHeader("Set-Cookie", connect->sessionCookie);
        connect->sessionCookie.clear();
    }
    cg->response =  respone.build();
    deleteClient(data, cg->fdOUT, connect->fdEp, "", "");
   
    if (flag == 0 || flag == -1 ){
        kill(cg->pid, SIGTERM);
        waitpid(cg->pid, NULL, WNOHANG );
    }
    connect->is_cgi = false;

    int sendB = send(connect->fd, cg->response.c_str(), cg->response.size(), 0);
    if (sendB <= 0){
        deleteClient(data, cg->fd_in, connect->fdEp, "", "");
        deleteClient(data, connect->fd, connect->fdEp, "", "");
        return ;
    }
    if (sendB < cg->response.size()){
        connect->response = cg->response.substr(sendB);
        deleteClient(data, cg->fd_in, connect->fdEp, "", "");
        return ;
    }
 
    deleteClient(data, cg->fd_in, connect->fdEp, "", ""); 
    if (!connect->keepAlive)
        deleteClient(data, connect->fd, connect->fdEp, " done ", connect->ipAddress);
    
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
    Response res;
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
    for(std::vector<int>::iterator it = ve.begin(); it != ve.end(); it++   ){
      
        if (findElement(config, *it) == "cgi") 
            handlingOfCgi(config, *it, 0, res);
        else
            deleteClient(config, *it, fdEp," timeout cleint ", "" );
    }
}

void checkClientConnection(maptype &config, Client &connect) {
    
    connect.currentTime = time(NULL);
  
    if (!connect.keepAlive ) {
       
        deleteClient(config, connect.fd, connect.fdEp, " done ", connect.ipAddress);
        return;
    }

    connect.buildDone = false;
    connect.requestFinish = false;
    connect.bodysize = 0;
    connect.byteSent = 0;
    connect.bodySizeStatus = false;    
    connect.parsedRequest = Request();
    setClientRead(connect.fdEp, connect);
}
void addCgi(maptype &data, Client &connect , pid_t pip,  int fdIN, int fdOUT){
    _Cgi *obj;

    obj = new _Cgi();

    obj->writeB = 0;
    fdIN = makeNonBlockingFD(fdIN);
    fdOUT = makeNonBlockingFD(fdOUT);
    memset(&obj->data, 0, sizeof(obj->data));
    obj->name = "cgi";
    obj->data.events = EPOLLIN  | EPOLLHUP | EPOLLERR;
    obj->data.data.fd = fdIN;
    obj->currentTime = time(NULL);
    obj->fd_in = fdIN;
    obj->pid = pip;
    obj->fdOUT = fdOUT;
    obj->serverId = connect.serverId;
    obj->fdEp = connect.fdEp;
    obj->connect = &connect;
    obj->writeB = write(fdOUT, connect.parsedRequest.body.c_str(), connect.parsedRequest.body.size());
    if (obj->writeB != (int)connect.parsedRequest.body.size()){
        obj->OUT.events = EPOLLOUT | EPOLLHUP | EPOLLERR;
        obj->OUT.data.fd = fdOUT;
        addSockettoEpoll(connect.fdEp, obj->OUT);
        data[fdOUT] = new _Cgi(*obj);
    }
    else{
        close(fdOUT);
    }

    obj->fd_client = connect.fd;
    addSockettoEpoll(connect.fdEp, obj->data);
    data[fdIN] = obj;
}

void sendResponse(maptype &config, Client &connect ) {
    
    int n = 1 ;
    Response respone;
    char buff[MAXSIZEBYTE] ;

    if (!connect.buildDone ){
        
        Server* srv = getServerFromClient(config, connect);
        respone.processRequest(connect.parsedRequest, *srv);
        if (!connect.sessionCookie.empty()) {
            respone.setHeader("Set-Cookie", connect.sessionCookie);;
            connect.sessionCookie.clear();
        }
        connect.response = respone.build();
        connect.buildDone = true;
        if (respone.isCgipending()){
            connect.response.clear();
            addCgi(config, connect, respone.getcgiPid(), respone.getcgiReadFd(), respone.getcgiWriteFd() );
            connect.currentTime = time(NULL);
            connect.is_cgi = true;
           
            return ;
        }
        if (respone.isLargeFile()){
            
            connect.fdFile = open(respone.getFilePath().c_str(), O_RDONLY);
            connect.fdsBuffer.push_back(connect.fdFile);
            connect.fdFile = makeNonBlockingFD(connect.fdFile);
            respone.getFilePath() = "";
        }
      
    }

    if (!connect.response.empty()){

        n = send(connect.fd, connect.response.c_str(), connect.response.size(), 0);
        if (n <= 0) {
           
            deleteClient(config, connect.fd, connect.fdEp, " send failed ",connect.ipAddress );
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
    if (connect.is_cgi)
        return ;
    if (connect.response.empty() || connect.fdFile == -1){
        connect.sending  = false ;
        checkClientConnection(config, connect);
        return ;
    }
    
    connect.sending = true;
  
    checkClientConnection(config, connect);
}
