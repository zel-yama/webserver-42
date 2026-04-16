

#include "include/webserv.hpp"
#include "include/Client.hpp"


Client::Client(){
    name = "client";
    requestFinish = false;
    bodySizeStatus = false;
    byteSent = 0;


    response  = "";
    sending = false;

    buildDone = false;
    fdFile = -1;


}
