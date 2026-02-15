

#include "../include/Server.hpp"
#include "../include/tools.hpp"
#include "../../request/RequestParser.hpp"
#include "../../Response/Response.hpp"

 ///should allawyz theck is connection timeout indeed  not just when u send response  
// allawyz check client for timeout in case timeout and close mod and i should 
// conclusesion now is monitor all  connection and i should disconnecte or rest  all expir ones  
//note all clients should know his server to get info from std

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
void handlingOFCGi(maptype &data, Server *srv, _Cgi *cg, Client *connect){
    
    const int  va = 15020;
    char buffer[va];
    int i = read(cg->fd_in, buffer, va );
    
    printf("cgi event \n");
    connect->response.append(buffer, i);
    srv->respone->applyCgiResponse(connect->response);
    connect->response =   srv->respone->build();
    kill(cg->pid, SIGTERM);

    printf("conn ->  %s }} \n", connect->response.c_str());
    sendResponse(data, *connect);


}

void eventLoop(maptype config ){
    
    int fdEp;
    Client *Cli = NULL;
    Client  newClient;
    Server *serv = NULL;
    _Cgi    *cgI = NULL;
    int n;
    
    fdEp = creatEpoll(config);
    
    struct epoll_event events[MAXEVENT];
    while(1){
        
        
        printf("-----epoll wait ---\n");
    
        printf("before EPOLL WAIT \n");
	    n = epoll_wait(fdEp, events, MAXEVENT,-1);
        printf("  and this epoll FD %d the size of config %lu and this event lengh %d \n", fdEp, config.size(), n);
        if (n == -1){
            throw runtime_error("error in epoll wait function ");}
       
            
            for(int i = 0; i < n; i++){
                
                if (findElement(config, events[i].data.fd) == "Server"){
                    
                    serv = dynamic_cast<Server *>(config.at(events[i].data.fd));
                    if (!serv)
                    continue;
                    
                    newClient =  serv->acceptClient();
                    config[newClient.fd] = &newClient;
                    cout << "accept " << newClient.fd << " from server " << serv->fd << endl; 
                    addSockettoEpoll(fdEp, newClient.data);               
                    continue; 
                }
                else  if (events[i].events & (EPOLLERR | EPOLLHUP)){
                    printf("close connection due to Error happens in client side ");
                    deleteClient(config, events[i].data.fd, fdEp);
                    continue;
                }
                else if (findElement(config, events[i].data.fd) == "cgi"){
        
           
                    cgI = dynamic_cast<_Cgi *>(config.at(events[i].data.fd));
                    Cli = dynamic_cast<Client *> (config.at(cgI->fd_client));
                    Server *serv = getServerFromClient(config, *Cli);
                    handlingOFCGi(config, serv, cgI, Cli);
                }
                else if (findElement(config, events[i].data.fd) == "client")         
                {
                    Cli = dynamic_cast<Client *>(config.at(events[i].data.fd));
                    Server *clientServer = getServerFromClient(config, *Cli);
                    if (!Cli || !clientServer)
                        continue;

                    if (checkTimeout(*Cli))
                        continue;
                    if (events[i].events & EPOLLIN ){
                      
                        std::cout << " fd cleint  "<< Cli->fd << std::endl;
                        if (!Cli->requestFinish)
                            readRequest(events[i].data.fd, *Cli, clientServer->parser);
                        
                        if  (Cli->requestFinish)
                            sendResponse(config, *Cli);
                        else 
                            continue;
                    }
                    if (events[i].events & EPOLLOUT  ) {
                        Cli = dynamic_cast<Client *>(config.at(events[i].data.fd));
                        sendResponse(config, *Cli);
                    }
                }
        }

        checkClientsTimeout(config, fdEp);
               
            }
        
        
    
    }


