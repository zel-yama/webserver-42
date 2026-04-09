
#ifndef CLIENT_HPP
#define CLIENT_HPP

// #include <iostream>
// #include <vector>
// #include <sys/socket.h>
// #include <sys/epoll.h>
#include "Server.hpp"
#include "Config.hpp"
// #include <arpa/inet.h>
class Client: public Config{

    public:
        Client();
        int fdEp;
        int fd;
        struct sockaddr_in ClientSock;

        time_t prevTime;
        bool keepAlive;
        bool requestFinish; 
        size_t bodysize ;
        std::string buffer;
        std::string response;
        bool sending;
        bool bodySizeStatus;

        int port;
        std::string ip;
    
        long byteRead;
        long byteSent;
    
        long bodysizeRequest;
        bool buildDone;
        int fdFile;

        std::string sessionCookie;

        Request parsedRequest;
        // Response respons;
};

#endif