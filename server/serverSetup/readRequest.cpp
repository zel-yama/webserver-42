#include "../include/Server.hpp"
#include "../include/tools.hpp"
#include "../../request/RequestParser.hpp"

int myread(Client &connect) {
    char tmp[1024];
    int n = 0;
 
    n = read(connect.fd, tmp, 1023);
    printf("this n of read byte in while {%d}\n", n);
    if (n < 0)
        return -1;
    if (n == 0)
        return 0;
  
    // tmp[n] = '\0';
    connect.byteSent += n;
//     std::string t = std::string(tmp, sizeof(tmp)); 
//             printf("t- > {%s}\n", t.c_str());//new how to append is can be problem in missing byte 
//     connect.buffer += tmp;
    connect.buffer.append(tmp, n);  // khas append just exactly n bytes
    
    return 1;
}

void readRequest(int fd, std::string& buffer, Client &connect, RequestParser *parser)
{
    // read only for body  () 
    int readResult = myread(connect);
    
    if (readResult == -1) {
        std::cerr << "Read error on fd " << fd << std::endl;
        return;
    }
    
    if (readResult == 0) {
        std::cout << "Client " << fd << " closed connection (read 0 bytes)" << std::endl;
        connect.requestFinish = false; 
        connect.keepAlive = false;
        return;
    }

    try {
        Request req = parser->parse(connect.fd, connect.buffer);

        if (req.complete) {
            connect.parsedRequest = req;
            connect.requestFinish = true;
        
            connect.keepAlive = !req.should_close;

            std::cout << "Request parsed successfully:" << std::endl;
            std::cout << "  Method: " << req.method << std::endl;
            std::cout << "  Path: " << req.path << std::endl;
            // std::cout << "Content-type" << req.headers["Content-Type"] << std::endl; // when we do at() curl response > curl -X POST http://localhost:8080/upload \-d "Hello world"
            //curl: (1) Received HTTP/0.9 when not allowed
            std::cout << "  Version: " << req.version << std::endl;
            std::cout << "  Status: " << req.status << std::endl;
            std::cout << "  Status: " << req.headers["content-length"] << std::endl;
            std::cout << "  Keep-Alive: " << (req.should_close ? "NO" : "YES") << std::endl;
            std::cout << "  Body size: " << req.body.size() << " bytes" << std::endl;
        } else {
            std::cout << "Request incomplete, waiting for more data..." << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "✗ Request parsing error: " << e.what() << std::endl;

        connect.parsedRequest.status = 400;
        connect.parsedRequest.complete = true;
        connect.requestFinish = true;
        connect.keepAlive = false;
    }
}