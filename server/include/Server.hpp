
#ifndef SERVER_HPP
#define SERVER_HPP


#define MAXEVENT 1000
#define MAXCLIENT 1000

// #include "../../webserv.hpp"

#include "../../Response/Response.hpp"
#include "Client.hpp"
#include "include.hpp"

struct location;
<<<<<<< HEAD
<<<<<<< HEAD
=======
class Client;
>>>>>>> origin

=======
// class Client;
// class Response;
>>>>>>> 1b3052310a3c050f0c43e700afad5866ff9f4475

typedef map<string, string>::iterator iter;
class Server : public Config {
    
    public:
    
    Server();
    
    std::vector<std::string>                 indexFile;
    unsigned int                        port;
    std::string                         ipAdress;
    std::string                         returnP;
    size_t                              bodyMaxByte;
    std::string                         root;
    // std::map<int,std::string>                error_pages;
        std::vector<std::string>            allowedMethods;
        bool                                outoIndex;
        int                                 fd;
        struct sockaddr_in                  addressServer;
        std::string                         ServerName;
        std::string                         ClientMaxBody;
        std::vector<location>               objLocation;
        std::map<int, std::string >         D_ErrorPages; //exit code with error page to that error 
<<<<<<< HEAD
=======

        // for me  (mohamed)
        RequestParser               parser;
<<<<<<< HEAD
>>>>>>> origin
=======
        Response respone;
>>>>>>> 1b3052310a3c050f0c43e700afad5866ff9f4475
        
        int                                 CreateServer(int port, std::string ipaddress );
        void                                listenFunction();
        Client                              acceptClient();
    };
    
typedef std::vector<Server> servers;
struct location {
 
    location();
    std::vector<std::string>            allowedMethods;
    std::string                         root;
    std::string                         returnP;
    std::string                         locationPath;
    std::vector<std::string>            indexFile;
    size_t                              bodyMaxByte;
    bool                                outoIndex;
    std::map<int, std::string >         D_ErrorPages;
};


int makeNonBlockingFD(int socket);
void eventLoop(maptype config );
int addSockettoEpoll(int fdEp, struct epoll_event  data);
size_t countBuffersize(std::string buffer, Client &connect);

<<<<<<< HEAD
=======
// Helper to get server from client (me again)
Server* getServerFromClient(maptype& config, Client& client);
>>>>>>> origin


#endif