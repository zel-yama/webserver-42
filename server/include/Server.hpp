#ifndef SERVER_HPP
#define SERVER_HPP


#define MAXEVENT 1000
#define MAXCLIENT 1000
#define MAXSIZEBYTE 165000

#include "Client.hpp"
#include "include.hpp"

struct location;
class Response;
class RequestParser;

typedef map<string, string>::iterator iter;
class Server : public Config {
    
    public:
    
    Server();
    
    std::vector<std::string>            indexFile;
    unsigned int                        port;
    std::string                         ipAdress;
    std::string                         returnP;
    size_t                              bodyMaxByte;
    std::string                         root;
    // std::map<int,std::string>                error_pages;
    int upload;
    std::string                          uploadPath;
    std::string                               cgiPath;
    std::string                          cgiExten;
    int                                     cgiStatus;
        std::vector<std::string>            allowedMethods;
        int                              outoIndex;
        int                                 fd;
        int                                 returnCode ;
        struct sockaddr_in                  addressServer;
        std::string                         ServerName;
        std::string                         ClientMaxBody;
        std::vector<location>               objLocation;
        std::map<int, std::string >         D_ErrorPages; //exit code with error page to that error 

        // for me  (mohamed)
        RequestParser               *parser;
        Response                    *respone;
        map<std::string, std::string>       cgiConfig;
        int                                 CreateServer(int port, std::string ipaddress );
        void                                listenFunction();
        Client                              acceptClient();
    };
    
typedef std::vector<Server> servers;
struct location {
 
    location();
    int                                  upload;
    map<std::string, std::string>        CgiCofing;
    std::string                          uploadPath;
    std::string                          cgiPath;
    std::string                          cgiExten;
    int                                  cgiStatus;
    std::vector<std::string>             allowedMethods;
    std::string                          root;
    std::string                          returnP;
    int                                  returnCode;
    std::string                          locationPath;
    std::vector<std::string>             indexFile;
    size_t                               bodyMaxByte;
    int                                  outoIndex;
    int                                  ex;
    std::map<int, std::string >          D_ErrorPages;
};


int makeNonBlockingFD(int socket);
void eventLoop(maptype config );
int addSockettoEpoll(int fdEp, struct epoll_event  data);
size_t countBuffersize(std::string buffer, Client &connect);

// Helper to get server from client (me again)
Server* getServerFromClient(maptype& config, Client& client);
void    costumThrow(std::string des, std::string invalid);

#endif