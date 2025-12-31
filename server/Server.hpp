
#ifndef SERVER_HPP
#define SERVER_HPP


#define MAXEVENT 1000
#define MAXCLIENT 1000
#include "../webserv.hpp"




class Server : public Config {

    public:
        Server();
        unsigned int port;
        std::string  ipAdress;
        size_t  bodyMaxByte;
        std::string root;

        int fd;
        struct sockaddr_in addressServer;
        string ServerName;
        string ClientMaxBody;
        vector<string> AllowMethods; 
        vector<pair<int, string > > D_ErrorPages; //exit code with error page to that error 
        vector<string>  split(string line, char delimiter);
        int  CreateServer(int port, string ipaddress );
        void listenFunction();
        Client acceptClient();
};

bool readRequest(int fd, std::string& buffer);
int makeNonBlockingFD(int socket);
void eventLoop(maptype config );
int addSockettoEpoll(int fdEp, struct epoll_event  data);







#endif