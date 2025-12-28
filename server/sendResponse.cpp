

#include "tools.hpp"
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
        //deleteClient(config, connect.fd,  connect.fdEp);
    }
}
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
    size_t bufferSize;
    while(1){
        size_t bufferSize = countBuffersize(connect.response, connect);
        len = send(connect.fd, connect.response.c_str(), bufferSize, 0);
        if (len == 0 || connect.response.size() == 0)
            return 0;
        if (len < 0)
            return -1;
        connect.response = connect.response.substr(len);
        
    }
    return -1;
}

void sendResponse(maptype &config, Client &connect){
    
    
    //size_t sent = countBuffersize(connect.response, connect);
    //size_t len = send(connect.fd, connect.response.c_str(), sent, 0);
    // if (len <= 0){

    //     return ;
    // }
    // if (connect.response.size() <= 0){
    //     connect.sending = false;
    // }
    // else if (len < connect.response.size()){
    //     connect.response = connect.response.substr(len);
    // }
    cout << "we in sending response ";
    if (mysend(connect) == 0)
        deleteClient(config, connect.fd, connect.fdEp);
    checkClientConnection(config, connect);
}