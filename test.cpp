#include <cctype>
#include <iostream>
#include "webserv.hpp"
void resolveIpName(std::string Str ){
    
    if (Str.empty())
        return ;
    int status;
    struct addrinfo *result = NULL;
    struct  addrinfo data;
    data.ai_family = AF_INET;
    data.ai_socktype = SOCK_STREAM;


    status = getaddrinfo(Str.c_str(), NULL, &data, &result);
    if (status)
        throw std::runtime_error("invalid host can't not be found [" + Str + "]");
    else
    {   
        struct addrinfo *tmp = result;
        while (tmp != NULL)
        {
            if (tmp->ai_family == AF_INET && tmp->ai_socktype == SOCK_STREAM){
            
                int fd = socket(AF_INET, SOCK_STREAM, 0);
                if (fd  != -1 ){
                    if (bind(fd, (struct sockaddr *)tmp->ai_addr, sizeof(addrinfo)) == -1)
                        printf("yes error in bind\n");
                    // listen()

                }

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