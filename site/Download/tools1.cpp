

#include "../../webserv.hpp"
#include "../include/tools.hpp"
// prevTime rest in accept or after response send if keep alive ;
// after this work and check performonc 
// that could be a problem if wait hanging system and will cause accept connection but without check timeout 
// void checkClientsTimeout(maptype& config, int fdEp) // fisrt for client after no cleint read or sent resposnse if timeout 

// after send response check connction status 

// u can control byte will send but just your buffer size should not be biger than string will send 
size_t countBuffersize(std::string buffer, Client &connect){
    
    if (buffer.size() >= connect.responseSize)
    {
        connect.sending = true;
        return connect.responseSize;
    }    
    else 
        return buffer.size();
}

int mysend(Client &connect){
    int len;

   
    size_t bufferSize = countBuffersize(connect.response, connect);
    len = send(connect.fd, connect.response.c_str(), bufferSize, 0);
    printf("this is lean size by send [%d]\n", len);
    if (len == 0 || connect.response.size() == 0)
        return 0;
    if (len < 0)
        return -1;
    connect.response = connect.response.substr(len);
    return 1;
}