#include "../include/Server.hpp"
#include "../include/tools.hpp"
#include "../../Response/Response.hpp"

void deleteCgi(maptype &data, _Cgi *cgi, int fdEp ){

    deleteClient(data, cgi->ErrorFD, fdEp, "", "");
    deleteClient(data, cgi->fd_in, fdEp, "", "");

}

void handlingOfCgi(maptype &data, int fd, int flag  ){
    
    static int  i =0;
    Response respone;
    char buffer[MAXSIZEBYTE] ;
 
    
    _Cgi *cg = (_Cgi *) returnElement( fd, data);
    if (!cg ) 
        return ;
    if (fd == cg->ErrorFD){
       int n = read(cg->ErrorFD, buffer, MAXSIZEBYTE);
       if (n < 0 || flag == 0){
            deleteClient(data, cg->ErrorFD, cg->fdEp, "", "");
            return ;
       }
        if (n > 0){
            _Cgi *cgtmp = (_Cgi *)returnElement(cg->fd_in, data);

            deleteClient(data, cg->ErrorFD, cg->fdEp, "", "");
            if (!cgtmp)
                return ;
            cgtmp->ErorrB = true;
           
            return ;
        }
        if (n == 0)
            return;
    }
    Client *connect = (Client *)cg->connect;
    if (!connect ){
      
     
        kill(cg->pid, SIGTERM);
        
        deleteClient(data, cg->fdOUT, cg->fdEp, "", "");
        deleteClient(data, cg->ErrorFD, cg->fdEp, "", "");
        deleteClient(data, cg->fd_in, cg->fdEp, "", "");
        return ;
    }
    Server *srv = (Server *)returnElement(connect->serverId, data);

 
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
    int status  = 0 ;
   
    int process = waitpid(cg->pid, &status, WNOHANG);
    (void)process;
    if (WIFEXITED(status) && WEXITSTATUS(status)  != 0){
        flag = 0;
    }
   
 
   
    if (flag == 1){
        long long i = read(cg->fd_in, buffer, MAXSIZEBYTE  );

        if (i < 0  ) {
            kill(cg->pid, SIGTERM);
            // deleteClient(data, cg->fdOUT, connect->fdEp, "", "");
            // deleteCgi(data, cg, connect->fdEp);
            // return ;
        }
        if (i > 0 ){
            connect->currentTime = time(NULL);
            cg->response.append(buffer, i);
           
            return ;
        }
    }
    
    if (flag == 0 || cg->ErorrB ){
      
        flag = -1;
        cg->response = "Status:500 Inter Server Error\r\n\r\n Error ";  
    }
    
    respone.setHeader("Server", srv->ServerName);
    if (!connect->keepAlive)
        respone.setHeader("Connection", "close");
    else
        respone.setHeader("Connection", "keep-alive");
    respone.logPath  = connect->parsedRequest.path;
    respone.logIpAdress = connect->ipAddress;
    respone.logMethod = connect->parsedRequest.method;
    respone.logUserAgent  = connect->parsedRequest.headers["user-agent"];
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
    i++;

    long long sendB = send(connect->fd, cg->response.c_str(), cg->response.size(), 0);
    if (sendB <= 0){
        deleteCgi(data, cg, connect->fdEp);
        deleteClient(data, connect->fd, connect->fdEp, "", "");
        return ;
    }
    if (sendB < (long long)cg->response.size()){
        connect->response = cg->response.substr(sendB);
        deleteCgi(data, cg, connect->fdEp);
        return ;
    }
 
    deleteCgi(data, cg, connect->fdEp);
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
   
    int max = 0;
    for (ConfigIter i = config.begin(); i != config.end(); i++) {
        
        if (max > 50)
            break;
        max++;
        if (i->second->name == "client") {
            connect = dynamic_cast<Client*>(i->second);
        
            if (checkTimeout(connect->currentTime, TIMEOUT)) {
                
                ve.push_back(connect->fd);
            }
        }
        if (i->second->name == "cgi"){
            _Cgi *cg = (_Cgi *) i->second;
            if (cg->ErorrB ||  checkTimeout(cg->currentTime, TIMEOUTCGI)){
                ve.push_back(cg->fd_in);
            }
        }
    }
    for(std::vector<int>::iterator it = ve.begin(); it != ve.end(); it++   ){
      
        if (findElement(config, *it) == "cgi") 
            handlingOfCgi(config, *it, 0);
        else
            deleteClient(config, *it, fdEp," [Timeout]  ", "" );
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
void addCgi(maptype &data, Client &connect , Response res){
    _Cgi *obj;

    obj = new _Cgi();

    obj->writeB = 0;
   
    memset(&obj->data, 0, sizeof(obj->data));
    obj->name = "cgi";
    
    obj->data.events = EPOLLIN  | EPOLLHUP | EPOLLERR;
    obj->currentTime = time(NULL);
    obj->fd_in = makeNonBlockingFD(res.cgiReadFd);
    obj->data.data.fd = obj->fd_in;
    obj->pid = res.cgiPid;
    obj->fdOUT = makeNonBlockingFD(res.cgiWriteFd);
    obj->serverId = connect.serverId;
    obj->fdEp = connect.fdEp;
    obj->ErrorFD =  makeNonBlockingFD(res.cgiError);;
    obj->connect = &connect;
  

    
    
    obj->writeB = write(obj->fdOUT, connect.parsedRequest.body.c_str(), connect.parsedRequest.body.size());
    if (obj->writeB != (int)connect.parsedRequest.body.size()){
        obj->OUT.events = EPOLLOUT | EPOLLHUP | EPOLLERR;
        obj->OUT.data.fd = obj->fdOUT;
        addSockettoEpoll(connect.fdEp, obj->OUT);
        data[obj->fdOUT] = new _Cgi(*obj);
    }
    else{
        close(obj->fdOUT);
    }

    obj->fd_client = connect.fd;
    addSockettoEpoll(connect.fdEp, obj->data);
    data[obj->fd_in] = obj;

    obj = new _Cgi(*obj);
    obj->Erorr.events = EPOLLIN  | EPOLLHUP | EPOLLERR;
    obj->Erorr.data.fd = obj->ErrorFD;
    
    data[obj->ErrorFD]  = obj;
    addSockettoEpoll(connect.fdEp, obj->Erorr);
}

void sendResponse(maptype &config, Client &connect) {
    
    int n = 1 ;
    Response respone;
    char buff[MAXSIZEBYTE] ;
   
    if (!connect.buildDone ){
        
        Server* srv = getServerFromClient(config, connect);
        respone.processRequest(connect.parsedRequest, *srv);
        if (!connect.sessionCookie.empty()) {
            respone.setHeader("Set-Cookie", connect.sessionCookie);
            connect.sessionCookie.clear();
        }
        connect.buildDone = true;
        if (respone.isCgipending()){
            connect.response.clear();
            addCgi(config, connect,  respone );
            connect.currentTime = time(NULL);
            connect.is_cgi = true;
            
            return ;
        }
        connect.response = respone.build();
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
           
            deleteClient(config, connect.fd, connect.fdEp, " [reseted] ",connect.ipAddress );
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
