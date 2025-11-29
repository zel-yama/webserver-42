
#include "Server.hpp"

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

void eventLoop(Server serv ){
    
    int fdEp;
    Client Cli;
    int n;
    char buffer[23];
    
    
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
                // here should check is server to accept client or client to read data it 
                //if request 
                    Cli =  serv.acceptClient();
                    cout << "accept " << Cli.fd << " from server " << serv.fd << endl;
  //                addSockettoEpoll();
                /// here if client should check client whom make event and u should read request if request is finish u should change status to EPULLOUT to monitoring to sending 
                    // response 
                    int j = read(events[i].data.fd,buffer, 23 );
                        cout << "we get some data " << endl;
                        // buffer[j-1] = '\0';
                        printf("this %s\n" , buffer);
            }
            if (events[i].data.fd & EPOLLOUT){
                // send response and change status to 
            }
        }
    }

}