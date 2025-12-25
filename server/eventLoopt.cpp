

#include "Server.hpp"
#include <stdexcept>
#include "tools.hpp"

 ///should allawyz theck is connection timeout indeed  not just when u send response  
// allawyz check client for timeout in case timeout and close mod and i should 
// conclusesion now is monitor all  connection and i should disconnecte or rest  all expir ones  
//note all clients should know his server to get info from him 
   




void sendResponse(int fdEp, int FSK, maptype &config){
     if(send(FSK, "<p>this is response </p><h1>this is title</h1>",47, MSG_DONTWAIT) == -1)
     {
                cerr << "error in send operatio" << endl;              
    } 
        cout << "here is delete socket "<< endl;
      
        deleteClient(config,FSK, fdEp);

}

void eventLoop(maptype config ){
    
    int fdEp;
    Client *Cli;
    Client  newClient;
    Server *serv;
    int n;
    
    fdEp = creatEpoll(config);
    struct epoll_event events[MAXEVENT];
    while(1){
    

	    n = epoll_wait(fdEp, events, MAXEVENT,-1);
        if (n == -1){
            throw runtime_error("error in epoll wait function ");}
        for(int i = 0; i < n; i++){
            if (events[i].events & EPOLLIN){
                if (config.at(events[i].data.fd)->name == "Server"){

                    serv = dynamic_cast<Server *>(config.at(events[i].data.fd));
                    newClient =  serv->acceptClient();
                    config.insert(pair<int,Config *>(newClient.fd, &newClient));
                    cout << "accept " << newClient.fd << " from server " << serv->fd << endl; 
                    addSockettoEpoll(fdEp,newClient.data);               
                    continue; 
                }
                if (config.at(events[i].data.fd)->name == "Client")
                {
                    Cli = dynamic_cast<Client *>(config.at(events[i].data.fd));
                   
                    if (readRequest(events[i].data.fd, Cli->buffer, *Cli))
                    {
                        setClientSend(fdEp, *Cli); // response big u should put it this 
                        sendResponse(fdEp, events[i].data.fd, config);
                       continue;
                    }
                }
            }
            if (events[i].events & EPOLLOUT ) {
                if(send(events[i].data.fd, "<p>this is response </p><h1>this is title</h1>",47, MSG_DONTWAIT) == -1){
                  cerr << "error in send operatio" << endl;
                   
                } 
                cout << "here is delete socket "<< endl;
      
                deleteClient(config,events[i].data.fd, fdEp);
            }
        }
    }

}

