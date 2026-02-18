


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
        perror("here: ");
        printf("%s", strerror(errno));
        return -1;
    }
    return 0;
}

void setClientSend(int fdEp,  Client &Clien){

    Clien.data.events = EPOLLOUT | EPOLLHUP | EPOLLERR;
    epoll_ctl(fdEp, EPOLL_CTL_MOD, Clien.fd, &Clien.data);
}

void setClientRead(int fdEp, Client& clien ){

    clien.data.events = EPOLLIN | EPOLLERR | EPOLLHUP ; 
    epoll_ctl(fdEp, EPOLL_CTL_MOD, clien.fd, &clien.data);
}

void  deleteClient(maptype& config, int fd, int fdEP){
    
   if (epoll_ctl(fdEP, EPOLL_CTL_DEL, fd, NULL)  == -1){
        return ;
   }
   __displayTime();
   std::cout << " close connection this  [" << fd << "]  \n " << std::endl;
    close(fd);
    maptype::iterator it = config.find(fd);
    
    if (it != config.end()){
        Client *c = (Client *) it->second;
        delete c;
        config.erase(fd);
    }
   
}
int creatEpoll( maptype& config){

    int fdEp;
   
    fdEp = epoll_create(8);
    if (fdEp == -1){
        throw std::runtime_error ("Error in creating epoll ");
    }
    for(ConfigIter it = config.begin(); it != config.end(); it++)
   { 
    
       
       Server *serv =   dynamic_cast<Server*>(it->second);
       if (!serv)
         continue;
       serv->fdEp = fdEp;
       serv->fdsBuffer.push_back(fdEp);
       serv->fdsBuffer.push_back(serv->fd);
       printf("sss %d\n", serv->data.data.fd);
        if (addSockettoEpoll(fdEp, serv->data) == -1)
            throw std::runtime_error("Error in add server to Epoll ");
   }
    return fdEp;
}
void __displayTime(){
    
    time_t current = time(NULL);
    struct tm local = *localtime(&current);
    printf("[%d-%d-%d %d:%d:%d]",local.tm_year, local.tm_mon, local.tm_mday, local.tm_hour, local.tm_min, local.tm_sec);

}
