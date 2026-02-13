
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
        int timeout;
        time_t prevTime;
        bool keepAlive;
        bool requestFinish; // if request finished return response 
        size_t bodysize ;
        std::string buffer;
        std::string response;
        bool sending;
        bool bodySizeStatus;
        bool headersOnly;
        //  std::vector<std::string> bufferRequest;
        // std::vector<std::string> contentResponse;
        long byteRead;
        long byteSent;
        long responseSize;
        long bodysizeRequest;
        bool buildDone;
        int fdFile;
        bool is_cgi;
        // i add this
        Request parsedRequest;
        // Response respons;
};

#endif