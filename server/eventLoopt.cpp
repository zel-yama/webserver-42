
#include "Server.hpp"
#include <stdexcept>

int readRequest(int fd){

    static vector<string> buff;
    char buffer[1280] ;
    int i = 0;
    string str;
    while(( i = recv(fd, buffer, 1280, MSG_DONTWAIT )) > 0){
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
// void acceptAddClient(Config &data, int FdEP){
    
// }
int creatEpoll( maptype config){

    int fdEp;
    fdEp = epoll_create(8);
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
        cout << "wait Epoll event " << endl;

	n = epoll_wait(fdEp, events, MAXEVENT,-1);
        if (n == -2){
            throw runtime_error("error in epoll wait function ");}
        for(int i = 0; i < n; i++){
            if (events[i].data.fd & EPOLLIN){
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
                        Cli->data.events = EPOLLOUT;
                        continue;
                    }
                }
            }
            if (events[i].data.fd & EPOLLOUT){
                if(send(events[i].data.fd, "yes this is reponse",47, MSG_DONTWAIT) == -1){
                  cerr << "error in send operatio" << endl;
                   continue;
                } 
                 close(events[i].data.fd);
            }
        }
    }

}
