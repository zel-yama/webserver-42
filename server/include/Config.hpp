

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

#endif
