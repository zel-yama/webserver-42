#include "../include/Server.hpp"
#include "../include/tools.hpp"
#include "../../request/RequestParser.hpp"
// you should read  request 
/// now u have fd that contain data  
// i should handle some case of reading body of request content length  
// timeout of reading of reading 
//  pass line by to parsing so 
// first read until u get of endof request and wait for body if exsits  and 
//   what happens when i read from socket and i came read next what will happns ? ofset 
// read requst u can know if body exists if content-length > 0 or post or put methods yes in case 
// diff between body exists  and no body 
// body without length in http u wait untill EOF or time end 
// body can so big so can't read in one time 
// close is will depend on time or not 
int myread(Client &connect, std::string& buffer) {
    
    char tmp[MAXSIZEBYTE];
    int byte = 0;
    int n = 0;

    n = read(connect.fd, tmp, MAXSIZEBYTE);

    if (n > 0) {
        connect.byteSent += n;
        buffer.append(tmp, n);   
        return n; 
    }
    if (n == 0) {
        return 0;
    }
    if (n < 0) {
        return n;
    }   
    

    return 1;
}

bool allowKeepAlive(Request req)
{
    if (req.status >= 400)
        return false;

    if (!req.keepalive)
        return false;


    if (!req.complete)
        return false;

    return true;
}

void readRequest(maptype &data,  int fd,  Client &connect, RequestParser *parser)
{
    int readResult = myread(connect, parser->buffer[fd]);
    
    printf("buffer %s\n", parser->buffer[fd].c_str());
    if (readResult <= 0) {
        std::cout << "Client " << fd << " closed connection (read 0 bytes)" << std::endl;
        deleteClient(data, fd, connect.fdEp);
        return;
    }
    if (readResult < 0 && connect.requestFinish) 
        return ;
    Request req = parser->parse(connect.fd, connect.buffer);

    if (req.complete) {
        connect.parsedRequest = req;
        connect.requestFinish = true;
    
        if (allowKeepAlive(req)) {
            connect.keepAlive = true;
            parser->requests[fd] = Request();
        }
        else {
            parser->requests.erase(fd);
            parser->buffer.erase(fd);
            connect.keepAlive = false;
        }
        cout << req.body.size() << endl;
        std::cout << "  Method: " << req.method << std::endl;
        std::cout << "  Path: " << req.path << std::endl;
        std::cout << "Content-type" << req.headers["Content-Type"] << std::endl;
        std::cout << "  Version: " << req.version << std::endl;
        std::cout << "  Status: " << req.status << std::endl;
        std::cout << "  Status: " << req.headers["content-length"] << std::endl;

   

    } else {
        std::cout << "Request incomplete, waiting for more data..." << std::endl;
        return ;
    }
}