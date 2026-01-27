#include <cctype>
#include <iostream>
#include <webserv.hpp>
void resolveIpName(std::string Str ){
    
    if (Str.empty())
        return ;
    int status;
    struct addrinfo *result = NULL;
    struct  addrinfo data;
    data.ai_family = AF_INET;
    data.ai_socktype = SOCK_STREAM;

    memset(&data, 0, sizeof(data));
    status = getaddrinfo(Str.c_str(), NULL, &data, &result);
    if (status)
        throw std::runtime_error("invalid host can't not be found [" + Str + "]");
    else
    {   
        struct addrinfo *tmp = result;
        while (tmp != NULL)
        {
            if (tmp->ai_family == AF_INET && tmp->ai_socktype == SOCK_STREAM){
                // void *ptr = ((sockaddr_in *)tmp->ai_addr)->sin_addr);
                std::cout << "here -> " <<  tmp->ai_canonname << "|"<< std::endl;
            }
            tmp = tmp->ai_next;
        }
        

    }

    
    freeaddrinfo(result);
}
int main(int ac , char *av[]){
    try{


        resolveIpName(av[1]);
    }
    catch(std::exception &e){
        std::cout << e.what() << std::endl;
    }
}