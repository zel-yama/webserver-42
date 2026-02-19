
#ifndef CLIENT_HPP
#define CLIENT_HPP

// #include <iostream>
// #include <vector>
// #include <sys/socket.h>
// #include <sys/epoll.h>
#include "../../request/RequestParser.hpp"
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

    
        long byteRead;
        long byteSent;
    
        long bodysizeRequest;
        bool buildDone;
        int fdFile;

        // i add this
        Request parsedRequest;
        // Response respons;
};

#endif