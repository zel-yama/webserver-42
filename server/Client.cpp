
#include "Client.hpp"

Client::Client(){
    name = "client";
    keepAlive = false;
    timeout = 40;
    prevTime = time(NULL);
    requestFinish = true;
    byteSent = 0;
}