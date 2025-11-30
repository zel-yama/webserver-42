
#include "Server.hpp"

int readRequest(int fd){

    if (1){
        return 1  ;
    }
    return 0;
}
int creatEpoll( maptype config){

    int fdEp;
    fdEp = epoll_create(5);
    if (fdEp == -1){
        cerr << "error in creating epoll " << errno << endl;
    }
    for(ConfigIter it = config.begin(); it != config.end(); it++)
   {
        
        Server *serv = dynamic_cast<Server*>(it->second);
       addSockettoEpoll(fdEp, serv->data);

   }
    return fdEp;
}

void eventLoop(maptype config ){
    
    int fdEp;
    Client *Cli;
    Client  newClient;
    Server *serv;
    int n;
    
    fdEp = creatEpoll(config);
    struct epoll_event events[MAXEVENT];
    cout << "************ event loop start ***********" << endl;
    while(1){
        n = epoll_wait(fdEp, events, MAXEVENT,-1);
        if (n == -1)
        {
            cerr << "error in epoll_wait  " << errno << endl;
            exit(1);
        }
        for(int i = 0; i < n; i++){
            if (events[i].data.fd & EPOLLIN){
                if (config.at(events[i].data.fd)->name == "Server"){
                    serv = dynamic_cast<Server *>(config.at(events[i].data.fd));
                    newClient =  serv->acceptClient();
                    config.insert(pair<int,Config *>(newClient.fd, &newClient));
                    cout << "accept " << newClient.fd << " from server " << serv->fd << endl; 
                }
                if (config.at(events[i].data.fd)->name == "Client")
                {
                    Cli = dynamic_cast<Client *>(config.at(events[i].data.fd));
                    if (readRequest(Cli->fd) == 1);
                        Cli->data.events = EPOLLOUT;
                }
                    
            }
            if (events[i].data.fd & EPOLLOUT){
                // send response and change status to 
            }
        }
    }

}