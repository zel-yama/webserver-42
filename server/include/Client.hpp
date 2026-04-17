
#ifndef CLIENT_HPP
#define CLIENT_HPP


#include "Server.hpp"
#include "Config.hpp"


class Client: public Config{

    public:
        Client();
        int                 fdEp;
        int                 fd;
        struct sockaddr_in ClientSock;
        time_t              prevTime;
        bool                keepAlive;
        bool                requestFinish; 
        size_t              bodysize ;
        std::string         response;
        bool                sending;
        bool                bodySizeStatus;
        int                 port;
        long                byteRead;
        long                byteSent;
        long                bodysizeRequest;
        bool                buildDone;
        int                 fdFile;
        std::string         ipAddress;
        std::string         sessionCookie;
        Request             parsedRequest;
      
};

#endif
