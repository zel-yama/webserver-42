
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"

class Client: public Config{

    public:
        Client();
        int fd;
        struct sockaddr_in ClientSock;
        time_t timeout;
        time_t prevTime;
        bool keepAlive;
        bool requestFinish; // if request finished return response 
        vector<string> bufferRequest;
        vector<string> contentResponse;
        long byteSent;

        

};

#endif