#ifndef TOOLS_HPP 
#define TOOLS_HPP
#include "Server.hpp"

void    setClientSend(int fdEp,  Client &Clien);
void    setClientRead(int fdEp, Client& clien );
void    deleteClient(maptype& config, int fd, int fdEP);
int     creatEpoll( maptype config);
bool    checkTimeout(long prevTime, long timeSec);
bool readRequest(int fd, std::string& buffer, Client &connect);
#endif