

#include "../include/Server.hpp"
#include "../include/tools.hpp"

#include "../../Response/Response.hpp"

 
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
    

    
    ConfigIter it = config.begin();
    Server* serv;
    size_t i = 0;
    if (it != config.end()){
            while( i < it->second->fdsBuffer.size()){
                close(it->second->fdsBuffer[i]);
                i++;
            }
            while(it != config.end()){
           
                Config *c = it->second;
          
            
                    delete c;

                it++;
            }


    }
    
    exit(0);
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
    Response res  ;
    
    int n;
    function(0);
    fdEp = creatEpoll(config);
    std::string info;
    struct epoll_event events[MAXEVENT];
    signal(SIGINT, signalhandler);
    while(1){
	    n = epoll_wait(fdEp, events, MAXEVENT, 5000);
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
                    addSockettoEpoll(fdEp, newClient->data);                 
                }
                else if (checkTimeout(config[events[i].data.fd]->currentTime, TIMEOUT) && 
                    findElement(config, events[i].data.fd)  == "client" ){
                        continue; 
                }
                else  if (events[i].events & (EPOLLERR | EPOLLHUP)){
                    if (findElement(config, events[i].data.fd) == "cgi" ){
                        handlingOfCgi(config, events[i].data.fd, 1, res);
                        continue;
                    }          
                    deleteClient(config, events[i].data.fd, fdEp);    
                }
                else if (findElement(config, events[i].data.fd) == "cgi"){
                 
                    if (checkTimeout(config[events[i].data.fd]->currentTime, TIMEOUTCGI))
                        continue;
                    if (events[i].events & EPOLLIN)
                        handlingOfCgi(config, events[i].data.fd, 1, res);
                    else if (events[i].events & EPOLLOUT)
                        handlingOfCgi(config, events[i].data.fd, 2, res);
                }
                else if (findElement(config, events[i].data.fd) == "client")         
                {
                    
                    Cli = (Client *)returnElement(events[i].data.fd ,config );
                    Server *clientServer = getServerFromClient(config, *Cli);
                    if (!clientServer)
                        continue;
                    if (events[i].events & EPOLLIN ){
                        if (!Cli->requestFinish)
                            readRequest(config, events[i].data.fd, *Cli, &clientServer->parser);
                        if (Cli->requestFinish)
                            setClientSend(fdEp, *Cli );
                        continue;
                    }
                    if (events[i].events & EPOLLOUT  ) {
                        Cli = (Client *) returnElement(events[i].data.fd, config);
                        sendResponse(config, *Cli, res);
                    }
                 
                }
            }
    
            checkClientsTimeout(config, fdEp);
        }
        
        
        
    }
    
    
