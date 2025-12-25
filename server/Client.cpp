
#include "Client.hpp"

Client::Client(){
    name = "client";
    keepAlive = false;
    timeout = 40;
    prevTime = time(NULL);
    requestFinish = false;
    bodySizeStatus = false;
    byteSent = 0;
    headersOnly = false;
}