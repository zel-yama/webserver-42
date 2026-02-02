


#include "../include/tools.hpp"

int makeNonBlockingFD(int socket){
    int flag = fcntl(socket, F_GETFL);
    fcntl(socket, F_SETFL,flag  | O_NONBLOCK );
    return (socket);
    }
int addSockettoEpoll(int fdEp, struct epoll_event  data){

    if (epoll_ctl(fdEp,  EPOLL_CTL_ADD, data.data.fd, &data) == -1)
    {
        cerr << "Error to add socket to epoll" << endl;
        return -1;
    }
    return 0;
}
void setClientSend(int fdEp,  Client &Clien){

    Clien.data.events =  EPOLLIN | EPOLLOUT;
    epoll_ctl(fdEp, EPOLL_CTL_MOD, Clien.fd, &Clien.data);
}
void setClientRead(int fdEp, Client& clien ){

    clien.data.events = EPOLLIN; 
    epoll_ctl(fdEp, EPOLL_CTL_MOD, clien.fd, &clien.data);
}
void deleteClient(maptype& config, int fd, int fdEP){
    
   if (epoll_ctl(fdEP, EPOLL_CTL_DEL, fd, NULL)  == -1){
    cerr << "Error in remove from fd Epoll " << errno << endl;
   }
   __displayTime();
   cout << " delete fd  [" << fd << "] from epoll \n ";
    close(fd);
    config.erase(fd);
}

int creatEpoll( maptype config){

    int fdEp;
    Config conf;
    fdEp = epoll_create(8);
    Server* serv;
    if (fdEp == -1){
        cerr << "error in creating epoll " << errno << endl;
    }
    for(ConfigIter it = config.begin(); it != config.end(); it++)
   { 
    
       
        Server *serv = dynamic_cast<Server*>(it->second);
        if (!serv)
            throw std::runtime_error("Error in caseting servers");
        addSockettoEpoll(fdEp, serv->data);
   }
    return fdEp;
}
void __displayTime(){
    
    time_t current = time(NULL);
    struct tm local = *localtime(&current);
    printf("[%d-%d-%d %d:%d:%d]",local.tm_year, local.tm_mon, local.tm_mday, local.tm_hour, local.tm_min, local.tm_sec);

}
void printInfo(string Info, string descr){

    __displayTime();
    printf("  [%s]  %s\n", Info.c_str(), descr.c_str());
}

bool checkTimeout(long prevTime, long timeSec){
    time_t currentTime = time(NULL);
    int diff = currentTime - timeSec;
    if (diff > 0)
        return true;
    
    return false;
}