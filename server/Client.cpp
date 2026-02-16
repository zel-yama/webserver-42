

#include "../webserv.hpp"
#include "include/Client.hpp"


Client::Client(){
    name = "client";
    keepAlive = false;
    timeout = 40; // there defoult but it would be a one in config file 
    requestFinish = false;// if request finish i should watch it to reading request ;
    bodySizeStatus = false;
    byteSent = 0;
    headersOnly = false;
    buffer = "";
    response  = "";
    sending = false;
    responseSize = 8000;
    buildDone = false;
    fdFile = -1;
    is_cgi = false;
    // mine ..... mohamed
    parsedRequest = Request();
    // respons = Response();
}
