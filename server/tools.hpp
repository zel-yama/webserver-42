#ifndef TOOLS_HPP 
#define TOOLS_HPP
#include "Server.hpp"

void setClientSend(int fdEp,  Client &Clien);
void setClientRead(int fdEp, Client& clien );
void deleteClient(maptype& config, int fd, int fdEP);

#endif