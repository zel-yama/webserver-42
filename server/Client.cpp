
#include "../webserv.hpp"
#include "include/Client.hpp"
Client::Client(){
    name = "client";
    keepAlive = false;
    timeout = 40; // there defoult but it would be a one in config file 
    prevTime = time(NULL);
    requestFinish = false;// if request finish i should watch it to reading request ;
    bodySizeStatus = false;
    byteSent = 0;
    headersOnly = false;
    buffer = "";
    response  =  "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: 86\r\n"
    "Connection: close\r\n"
    "\r\n"
    "<html><body><h1>Hello!</h1><p>This is a random response.</p></body></html>";
    sending = false;
    responseSize = 8000;
<<<<<<< HEAD
}
   
=======

    // mine ..... mohamed
    parsedRequest = Request();
}
>>>>>>> origin
