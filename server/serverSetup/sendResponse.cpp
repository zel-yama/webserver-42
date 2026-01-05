


#include "../include/Server.hpp"
#include "../include/tools.hpp"
// prevTime rest in accept or after response send if keep alive ;
// after this work and check performonc 
// that could be a problem if wait hanging system and will cause accept connection but without check timeout 
void checkClientsTimeout(maptype& config, int fdEp) // fisrt for client after no cleint read or sent resposnse if timeout 
{

    Config data;
    Client *connect;
    for(ConfigIter i = config.begin(); i != config.end(); i++){
        
        if (i->second->name == "client"){
            connect = dynamic_cast<Client *>(i->second);
            if ((time(NULL) - connect->prevTime) > (connect->timeout - 10)){
                __displayTime();
                printf("Client [%d] close to timeout about 10 \n", connect->fd);
            }
            if ((time(NULL) - connect->prevTime) > (connect->timeout + 2)){
                    deleteClient(config, connect->fd, fdEp);
                    return;
            }
        }
    }
}
// after send response check connction status 

void checkClientConnection(maptype &config, Client &connect){

    if (connect.sending)
    {
        connect.buffer = "";
        setClientSend(connect.fdEp, connect); // response big u should put it this 
        return ;
    }
    if (connect.keepAlive){
        connect.prevTime = time(NULL);
        connect.requestFinish = false;
        connect.headersOnly = false;
        connect.bodysize   = 0;
        connect.byteSent = 0;
        connect.bodySizeStatus = false;
        connect.buffer = "";
        connect.keepAlive = false;
        setClientRead(connect.fdEp, connect);
        return;
    }
    if (connect.keepAlive == false){
        deleteClient(config, connect.fd,  connect.fdEp);
    }
}


void sendResponse(maptype &config, Client &connect){
    
    int n = mysend(connect);
  
    if (n == 0)
        connect.sending = false;
    
    checkClientConnection(config, connect);
}