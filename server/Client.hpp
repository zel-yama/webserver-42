
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"

class Client{

    public:
        int fd;
        struct sockaddr_in ClientSock;
    

};

#endif