

#include "Server.hpp"



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
    return buf.find("\r\n\r\n") != std::string::npos;
}

bool hasHeaderBody(const std::string &buf){
    return buf.find("Content-length:") != std::string::npos;
}
size_t getBodySize(const char *content, int pos){
    size_t size = 0;
    char  c;
    while(isdigit(content[pos])){
        c  = content[pos];
        size * 10 + (c - 48);
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
    char *tmp;
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


void readRequest(int fd, std::string& buffer, Client &connect)
{
    char tmp[4096];

    int n  = recv(fd, tmp + connect.byteSent, sizeof(tmp), 0);
    if (n == 0 ){
        if (hasHeaderEnd(buffer)){
            if (checkPost(buffer))
                connect.requestFinish = true;
        }
    }
    if (connect.byteSent <= 0){
        return ; // client closed or error
    }
    connect.buffer.append(tmp, connect.byteSent);
    connect.byteSent += n;
    if (hasHeaderEnd(tmp)){
        if (hasHeaderBody(tmp) && getContentLength(connect.buffer, connect)) {
            readBody(fd, connect);
        }
        if (!checkPost(connect.buffer))
            connect.requestFinish = true;
    }

}