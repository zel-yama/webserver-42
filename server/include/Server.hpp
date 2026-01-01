
#ifndef SERVER_HPP
#define SERVER_HPP


#define MAXEVENT 1000
#define MAXCLIENT 1000

#include "../../webserv.hpp"

#include "Client.hpp"
#include "include.hpp"

class location;


typedef map<string, string>::iterator iter;
class Server : public Config {

    public:
        Server();
        std::vector<std::string>                 indexFile;
        unsigned int                        port;
        std::string                         ipAdress;
        size_t                              bodyMaxByte;
        std::string                         root;
        std::map<int,std::string>                error_pages;
        std::vector<std::string>                 allowedMethods;
        bool                                outoIndex;
        int                                 fd;
        struct sockaddr_in                  addressServer;
        std::string                              ServerName;
        std::string                              ClientMaxBody;
        std::vector<location>               objLocation;
        std::vector<std::pair<int, std::string > >         D_ErrorPages; //exit code with error page to that error 
        
        int                                 CreateServer(int port, std::string ipaddress );
        void                                listenFunction();
        Client                              acceptClient();
};

class location : public Server{

    

    
};


int makeNonBlockingFD(int socket);
void eventLoop(maptype config );
int addSockettoEpoll(int fdEp, struct epoll_event  data);
size_t countBuffersize(std::string buffer, Client &connect);



#endif