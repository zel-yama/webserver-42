
#include "Server.hpp"
#include <stdexcept>
#include "tools.hpp"
int readRequest(int fd){

    static vector<string> buff;
    char buffer[1280] ;
    int i = 0;
    string str;
    while((i = recv(fd, buffer, 1280, MSG_DONTWAIT )) > 0){
        buffer[i] = '\0';

        buff.push_back(buffer);
    }
    for(vector<string>::iterator it = buff.begin(); it != buff.end(); it++){
        str = *it;
        cout << *it << endl;
        if (strstr(str.c_str(), "\r\n\r\n"))
            return 1;
    }
    return 1;
}


void sendResponse(int fdEp, int FSK, maptype &config){
    
    
     if(send(FSK, "<p>this is response </p><h1>this is title</h1>",47, MSG_DONTWAIT) == -1)
     {
                cerr << "error in send operatio" << endl;              
    } 
        cout << "here is delete socket "<< endl;
        if (checkTimeout())
            deleteClient(config,FSK, fdEp);

}
void e
 
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
                   
                    if (readRequest(Cli->fd) == 1)
                    {
                        setClientSend(fdEp, *Cli);
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

