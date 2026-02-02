#include "../include/Server.hpp"
#include "../include/tools.hpp"
#include "../../request/RequestParser.hpp"

<<<<<<< HEAD
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

#include <cctype>
#include <string>


bool hasHeaderEnd(const std::string& buf)
{
    std::cout << "---------------------------"<< std::endl;

    if (buf.find("\n\n") != std::string::npos || buf.find("\r\n\r\n") != std::string::npos)//new 
        return true;
    return false;
}

bool hasHeaderBody(const std::string &buf){
    return buf.find("Content-length:") != std::string::npos;
}
size_t getBodySize(const char *content, int pos){
    size_t size = 0;
    char  c;
    while(isdigit(content[pos])){
        c  = content[pos];
        size = size * 10 + (c - 48);
    }
    return size;
}
bool checkPost(const std::string &headers){
    return (headers.find("POST") != std::string::npos);
}
bool getContentLength(const std::string& headers, Client &connect)
{
    std::string key = "Content-Length:";
    size_t  pos = headers.find("Content-length:");
 
    connect.bodysize = getBodySize( key.c_str(), pos + key.size());
    if (connect.bodysize > 0)
        return true;
    return false; 
}


void readBody(int fd, Client &connect ){
    char *tmp = NULL;
    int n  = recv(fd, tmp, connect.bodysize, MSG_DONTWAIT);
    if (n <= 0)
        return ;
    connect.buffer.append(tmp, connect.byteSent );
    n = recv(fd, tmp, 1, MSG_DONTWAIT);
    if (n > 0)
        connect.bodySizeStatus = true;
    else 
        connect.bodySizeStatus = false;
}
void printDebug(Client Connect){
 
    printf("this is buffer | %s |\n", Connect.buffer.c_str());
    std::cout << "byte sent " << Connect.byteSent << std::endl;
    std::cout << "bodysize " << Connect.bodysize << std::endl;
    std::cout << "bodySizeStatus " << Connect.bodySizeStatus << std::endl;
    std::cout << "byteRead  " << Connect.byteRead << std::endl;
    std::cout << "fd  " << Connect.fd << std::endl;

}
int myread(Client &connect){
=======
int myread(Client &connect) {
>>>>>>> origin/master
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

/*
EAGAIN	no data now, try later 
ola khdm be :
EWOULDBLOCK	same meaning
*/

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

<<<<<<< HEAD
void readRequest(int fd,  Client &connect)
=======
bool allowKeepAlive(const Request& req)
>>>>>>> origin/master
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

    try {
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
            std::cout << "  Status: " << req.headers["content-length"] << std::endl;
            std::cout << "  Keep-Alive: " << (req.keepalive ? "YES" : "NO") << std::endl;
            std::cout << "  Body size: " << req.body.size() << " bytes" << std::endl;

            // khask tb3 multi part for debag
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