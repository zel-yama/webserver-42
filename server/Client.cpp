

#include "../webserv.hpp"
#include "include/Client.hpp"


Client::Client(){
    name = "client";
    requestFinish = false;// if request finish i should watch it to reading request ;
    bodySizeStatus = false;
    byteSent = 0;

    buffer = "";
    response  = "";
    sending = false;

    buildDone = false;
    fdFile = -1;

    // mine ..... mohamed
    
    // respons = Response();
}
