

#include "../include/Server.hpp"
#include "../include/tools.hpp"
// #include "../../request/RequestParser.hpp"
#include "../../Response/Response.hpp"

 ///should allawyz theck is connection timeout indeed  not just when u send response  
// allawyz check client for timeout in case timeout and close mod and i should 
// conclusesion now is monitor all  connection and i should disconnecte or rest  all expir ones  
//note all clients should know his server to get info from std
int function(int va){
  static int var; 
    if (va == 0)
        return var;
    var = va;
    return var;
}
void signalhandler(int sig){
    (void)sig;
    function(1);
}

std::string findElement(maptype &config, int fd){
    std::string str = "NOT FOUND";
    if (config.empty())
        return str;
    maptype::iterator it = config.find(fd);

    if (it != config.end() && it->second)
        return it->second->name;
    else
        return str;
}
void cleanUP(maptype &config){
    
    _Cgi *cg;
    Server *s;
    Client *c;
    ConfigIter it = config.begin();
    size_t i = 0;
    if (it != config.end()){
            while( i < it->second->fdsBuffer.size()){
                close(it->second->fdsBuffer[i]);
                i++;
            }
            while(it != config.end()){
                if (it->second->name == "cgi"){
                    cg = (_Cgi *) it->second;
                    delete  cg;
                }
                if (it->second->name == "Server"){
                    s  = (Server *) it->second;
        
                    delete s->respone;
            
                    
                    
                }
                if (it->second->name == "Client"){
                    c = (Client *) it->second;
                    
        
                }
                it++;
            }


    }
    std::cout << "CONTRL+C" << std::endl;
    exit(1);
}
Config *returnElement(int fd, maptype &data){
    if (data.count(fd) == 0)
        return NULL;

    return data.at(fd);
}

void eventLoop(maptype &config ){
    
    int fdEp;
    Client *Cli = NULL;
    Client*  newClient;
    Server *serv = NULL;
    _Cgi    *cgI = NULL;
    int n;
    function(0);
    fdEp = creatEpoll(config);
    
    struct epoll_event events[MAXEVENT];
    signal(SIGINT, signalhandler);
    while(1){
        
        
        
        printf("----- epoll wait -----\n");
        
        printf("before EPOLL WAIT \n");
	    n = epoll_wait(fdEp, events, MAXEVENT,5000);
           if (function(0) == 1 || n == -1){
               cleanUP(config);

           }
         
            for(int i = 0; i < n; i++){
               
                if (config.count(events[i].data.fd) == 0){ 
                         
                    continue;
                }

                else if (findElement(config, events[i].data.fd) == "Server"){
                    
                
                    serv = (Server *)returnElement(events[i].data.fd, config);  
                    newClient =  new Client(serv->acceptClient());
                    config[newClient->fd] = newClient;
                    cout << "Accept [client] [ " << newClient->fd << " ] from server " << serv->fd << endl; 
                    addSockettoEpoll(fdEp, newClient->data);               
                   
                }
                else if (checkTimeout(config[events[i].data.fd]->currentTime, TIMEOUT) && 
                                    findElement(config, events[i].data.fd)  == "client" ){
               
                  continue;

                }
                else  if (events[i].events & (EPOLLERR | EPOLLHUP)){
                    
                    if (findElement(config, events[i].data.fd) == "cgi" ){
                        handlingOFCGi(config, events[i].data.fd, 1);
                        continue;
                    }
                    
                    printf("close connection due to Error happens in client side ");
                    deleteClient(config, events[i].data.fd, fdEp);    
                }
                else if (findElement(config, events[i].data.fd) == "cgi"){//new pwd 
            
                    if (checkTimeout(config[events[i].data.fd]->currentTime, TIMEOUTCGI))
                        continue;
                    if (events[i].events & EPOLLIN)
                        handlingOFCGi(config, events[i].data.fd, 1);
                    else if (events[i].events & EPOLLOUT)
                        handlingOFCGi(config, events[i].data.fd, 2);
                    
                }
                else if (findElement(config, events[i].data.fd) == "client")         
                {
                    Cli = (Client *)returnElement(events[i].data.fd ,config );
                    Server *clientServer = getServerFromClient(config, *Cli);
                    if (!clientServer)
                        return ;
                    if (events[i].events & EPOLLIN ){
                        
                        std::cout << " fd cleint  "<< Cli->fd << std::endl;
                        if (!Cli->requestFinish)
                            readRequest(config, events[i].data.fd, *Cli, &clientServer->parser);
                        if (Cli->requestFinish)
                            setClientSend(fdEp, *Cli);
                        // if  (Cli->requestFinish)
                        //     sendResponse(config, *Cli);
                        // else 
                            continue;
                    }
                    if (events[i].events & EPOLLOUT  ) {
                        Cli = (Client *) returnElement(events[i].data.fd, config);
                        sendResponse(config, *Cli);
                    }
                }
            }
    
            checkClientsTimeout(config, fdEp);
        }
        
        
        
    }
    
    
