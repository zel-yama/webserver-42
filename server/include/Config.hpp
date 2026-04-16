

#ifndef CONFIG_HPP
#define CONFIG_HPP 

#include <iostream>

#include "webserv.hpp"

class Config{
    public:
        static std::vector<int> fdsBuffer;
        int fdEp;
        std::string name ;
        struct epoll_event data;
        time_t currentTime;
        bool is_cgi;
        int serverId;
        Config();
        virtual ~Config(){}
};



class _Cgi : public Config{

    public:
        int fd_client;
        struct epoll_event OUT;
        int fd_in;
        int fdEp;
        int ErrorFD;
        struct epoll_event Erorr;
        bool ErorrB;
        int writeB;
        int fdOUT;
        pid_t pid;
        std::string response;
        void *connect;
        _Cgi();
        
    
};

#endif

