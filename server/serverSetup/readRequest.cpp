

#include "../include/Server.hpp"
#include "../include/tools.hpp"


int myread(Client &connect, std::string& buffer) {
    
    char tmp[MAXSIZEBYTE];

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

bool allowKeepAlive(Request& req)
{
    if (req.status >= 400)
        return false;

    if (!req.keepalive)
        return false;


    if (!req.complete)
        return false;

    return true;
}

int  readRequest(maptype &data,  int fd,  Client &connect, RequestParser *parser)
{
    int readResult = myread(connect, parser->buffer[fd]);
    

   
    if (readResult <= 0) {
       
        connect.requestFinish = false;
        deleteClient(data, fd, connect.fdEp, "read failed ", connect.ipAddress );
        return -1;
    }

    Request req = parser->parse(connect.fd);

    if (req.complete) {
        req.ip = connect.ipAddress;
        connect.parsedRequest = req;
        connect.sessionCookie = parser->resolveSession(req);
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
        // std::cout << req.body.size() << std::endl;
        // std::cout << req. << std::endl;
    } 
    return 0;
}