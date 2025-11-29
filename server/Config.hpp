

#ifndef CONFIG_HPP
#define CONFIG_HPP 

#include <iostream>

class Config{
    public:
        std::string name ;
        struct epoll_event data;
        virtual ~Config(){}
};

#endif
