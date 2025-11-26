
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"

class Client: public Config{

    public:
        string name ;
        int fd;
        struct sockaddr_in ClientSock;
        struct epoll_event data;

};

#endif