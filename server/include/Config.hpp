

#ifndef CONFIG_HPP
#define CONFIG_HPP 

#include <iostream>

#include "../../webserv.hpp"

class Config{
    public:
        static std::vector<int> fdsBuffer;
        int fdEp;
        std::string name ;
        struct epoll_event data;
        time_t currentTime;
        
        int serverId;
        virtual ~Config(){}
};



class _Cgi : public Config{

    public:
        int fd_client;
        int fd_in;
        int fdEp;
        pid_t pid;
        
    
};

#endif

