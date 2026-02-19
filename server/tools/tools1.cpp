

#include "../../webserv.hpp"
#include "../include/tools.hpp"
// prevTime rest in accept or after response send if keep alive ;
// after this work and check performonc 
// that could be a problem if wait hanging system and will cause accept connection but without check timeout 
// void checkClientsTimeout(maptype& config, int fdEp) // fisrt for client after no cleint read or sent resposnse if timeout 

// after send response check connction status 

// u can control byte will send but just your buffer size should not be biger than string will send 

void    costumThrow(std::string des, std::string invalid){

    throw std::runtime_error(des + " -> [" + invalid + "]");
}

