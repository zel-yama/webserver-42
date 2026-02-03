#include "../include/Server.hpp"
#include "../include/tools.hpp"
#include "../../Response/Response.hpp"

Server* getServerForClient(maptype& config, int serverId);

void checkClientsTimeout(maptype& config, int fdEp) 
{
    Config *data;
    Client *connect;
    
    for (ConfigIter i = config.begin(); i != config.end(); i++) {
        if (i->second->name == "client") {
            connect = dynamic_cast<Client*>(i->second);
            
            if ((time(NULL) - connect->prevTime) > (connect->timeout - 10)) {
                printf("Client [%d] close to timeout about 10s\n", connect->fd);
            }
            if ((time(NULL) - connect->prevTime) > (connect->timeout + 2)) {
                deleteClient(config, connect->fd, fdEp);
                return;
            }
        }
    }
}

void checkClientConnection(maptype &config, Client &connect) {
    if (connect.sending) {
        connect.buffer = "";
        setClientSend(connect.fdEp, connect);
        return;
    }
    
    // Check if connection should close (from parsed request)
    if (!connect.keepAlive) {
        deleteClient(config, connect.fd, connect.fdEp);
        return;
    }
    
    // Keep-alive: reset for next request
    connect.prevTime = time(NULL);
    connect.requestFinish = false;
    connect.headersOnly = false;
    connect.bodysize = 0;
    connect.byteSent = 0;
    connect.bodySizeStatus = false;
    connect.buffer = "";
    
    connect.parsedRequest = Request();
    
    setClientRead(connect.fdEp, connect);
}

void sendResponse(maptype &config, Client &connect) {
    
    // Get the server configuration

    Server* srv = getServerForClient(config, connect.serverId);
    srv->respone->processRequest(connect.parsedRequest, *srv);
    std::string response = srv->respone->build();

   // std::cout << response << "\n";
    int n = send(connect.fd, response.c_str(), response.size(), 0);
    
    if (n <= 0) {
        deleteClient(config, connect.fd, connect.fdEp);
        return;
    }
    
    std::cout << "Sent " << n << " bytes (HTTP " << connect.parsedRequest.status << ")" << std::endl;
    
    connect.sending = (n < (int)response.length());
    
    checkClientConnection(config, connect);
}