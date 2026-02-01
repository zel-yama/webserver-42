#include "../include/Server.hpp"
#include "../include/tools.hpp"
#include "../../request/RequestParser.hpp"

int myread(Client &connect) {
    char tmp[1024];
    
    while (true) {
        int n = 0;
    
        n = read(connect.fd, tmp, 1023);
        if (n > 0) {
            printf("this n of read byte in while {%d}\n", n);
            connect.byteSent += n;
            connect.buffer.append(tmp, n);
            continue;
        }

        if (n == 0) {
            return 0;
        }

        if (n < 0) {
            if (errno == EAGAIN) {
                break ;
            } else {
                perror("read");
                return -1;
            }
        }
    }
    
    return 1;
}

bool allowKeepAlive(const Request& req)
{
    if (req.status >= 400)
        return false;

    if (!req.keepalive)
        return false;


    if (!req.complete)
        return false;

    return true;
}

void readRequest(int fd, std::string& buffer, Client &connect, RequestParser *parser)
{
    int readResult = myread(connect);
    
    if (readResult == -1) {
        return;
    }
    
    if (readResult == 0) {
        std::cout << "Client " << fd << " closed connection (read 0 bytes)" << std::endl;
        connect.requestFinish = false; 
        connect.keepAlive = false;
        return;
    }

    Request req = parser->parse(connect.fd, connect.buffer);

    if (req.complete) {
        connect.parsedRequest = req;
        connect.requestFinish = true;
    
        if (allowKeepAlive(req))
            connect.keepAlive = true;
        else
            connect.keepAlive = false;

        std::cout << "Request parsed successfully:" << std::endl;
        std::cout << "  Method: " << req.method << std::endl;
        std::cout << "  Path: " << req.path << std::endl;
        std::cout << "  Version: " << req.version << std::endl;
        std::cout << "  Status: " << req.status << std::endl;
        std::cout << "  content-type: " << req.headers["content-type"]  << std::endl;
        std::cout << "  content-length: " << req.headers["content-length"] << std::endl;
        std::cout << "  Keep-Alive: " << (req.keepalive ? "YES" : "NO") << std::endl;
        std::cout << "  Body size: " << req.body.size() << " bytes" << std::endl;

    } else {
        std::cout << "Request incomplete, waiting for more data..." << std::endl;
    }
}