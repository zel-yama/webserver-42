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
    if (connect.response.empty() && connect.fdFile == 0)
        connect.sending = false;
    if (connect.response.empty() && connect.fdFile == -1)
        connect.sending = false;

        
    if (connect.sending) {
        connect.buffer = "";
        connect.prevTime = time(NULL);
        setClientSend(connect.fdEp, connect);
        return;
    }
    

    // Check if connection should close (from parsed request)
    if (!connect.keepAlive) {
        deleteClient(config, connect.fd, connect.fdEp);
        return;
    }
    
    // Keep-alive: reset for next request
    connect.buildDone = false;
    
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
    int n = 1 ;
    int readB = 0;
    char buff[MAXSIZEBYTE];
   
    if (!connect.buildDone){
        Server* srv = getServerForClient(config, connect.serverId);
        srv->respone->processRequest(connect.parsedRequest, *srv);
        connect.response = srv->respone->build();
        connect.buildDone = true;
        if (srv->respone->isLargeFile()){
            connect.fdFile = open(srv->respone->getFilePath().c_str(), O_RDONLY);
            connect.fdFile = makeNonBlockingFD(connect.fdFile);
        }

    }
    int byte = 0;
    while(true){
        if (connect.fdFile != -1){
            byte =  read(connect.fdFile, buff, MAXSIZEBYTE);
            if (byte > 0)
                connect.response.append(buff, byte);
        }
        printf("response | %s |\n", connect.response.c_str());
        n = send(connect.fd, connect.response.c_str(), connect.response.size(), 0);
        if (n < 0  || connect.response.empty())
            break;
        if (n == 0) {
            deleteClient(config, connect.fd, connect.fdEp);
            break;
        }
        if (n > 0)
            connect.response.erase(0, n);

    }
    
    
    if (connect.response.size() > 0 || byte > 0 )
        connect.sending = true;
    std::cout << "Sent " << n << " bytes (HTTP " << connect.parsedRequest.status << ")" << std::endl;
    
    checkClientConnection(config, connect);
}