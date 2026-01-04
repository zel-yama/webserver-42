#ifndef TOOLS_HPP 
#define TOOLS_HPP


#include "../../webserv.hpp"
#include "include.hpp"
#include "Client.hpp"
#include "Server.hpp"

void    setClientSend(int fdEp,  Client &Clien);
void    setClientRead(int fdEp, Client& clien );
void    deleteClient(maptype& config, int fd, int fdEP);
int     creatEpoll( maptype config);
bool    checkTimeout(long prevTime, long timeSec);

void         __displayTime();
void        sendResponse(maptype &config, Client &connect);

int     mysend(Client &connect);
void    readRequest(int fd, std::string& buffer, Client &connect);
void    checkClientsTimeout(maptype& config, int fdEp);
void     checkClientConnection(maptype &config, Client &connect);
void    printAllConfig(std::vector<Server> servs);
#endif