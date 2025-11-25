
#include "Server.hpp"

void eventLoop(Server serv ){
    
    int fdEp;
    Client Cli;
    int n;
    char buffer[23];
    
    fdEp = epoll_create(5);
    if (fdEp == -1){
        cerr << "error in creating epoll " << errno << endl;
    }
    //add server to epoll 
   if ( epoll_ctl(fdEp,  EPOLL_CTL_ADD, serv.fd, &serv.data) == -1)
    {
        cerr << "error in epoll_ctl  " << errno << endl;
    }
    struct epoll_event events[10];
    cout << "************ event loop start ***********" << endl;
    while(1){
        n = epoll_wait(fdEp, events, 10, 1515);
        if (n == -1)
        {
            cerr << "error in epoll_wait  " << errno << endl;
            exit(1);
        }
        for(int i = 0; i < n; i++){
            if (events[i].data.fd & EPOLLIN){
               Cli =  serv.acceptClient();
               cout << "accept " << Cli.fd << " from server " << serv.fd << endl;
              if (epoll_ctl(fdEp, EPOLL_CTL_ADD, Cli.fd, &Cli.data) == -1)
                cerr << "error in accept client " << errno << endl;
                int j = read(events[i].data.fd,buffer, 23 );
                    cout << "we get some data " << endl;
                    // buffer[j-1] = '\0';
                   printf("this %s\n" , buffer);
            }
        }
    }

}