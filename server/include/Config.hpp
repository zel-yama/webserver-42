

#ifndef CONFIG_HPP
#define CONFIG_HPP 

#include <iostream>

#include "../../webserv.hpp"

class Config{
    public:
        int fdEp;
        std::string name ;
        struct epoll_event data;
        time_t currentTime;
        
        int serverId;
        virtual ~Config(){}
};

// class File : public Config{
    
//     public:

//         std::string buffer;
//         int bytsent;
//         int fd;
    
// };

class _Cgi : public Config{

    public:
        int fd_client;
        int fd_in;
        int fdEp;
        pid_t pid;
        
    
};

#endif

