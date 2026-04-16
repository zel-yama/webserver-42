#ifndef TOOLS_HPP 
#define TOOLS_HPP



#include "include.hpp"
#include "Client.hpp"
#include "Server.hpp"

void        setClientSend(int fdEp,  Client &Clien);
void        setClientRead(int fdEp, Client& clien );
void        deleteClient(maptype& config, int fd, int fdEP, std::string des, std::string ipAdd);
int         creatEpoll( maptype &config);
void        sendResponse(maptype &config, Client &connect);
int         readRequest(maptype &data, int fd, Client &connect, RequestParser *parser);
void        checkClientsTimeout(maptype& config, int fdEp);
void        checkClientConnection(maptype &config, Client &connect);


#endif