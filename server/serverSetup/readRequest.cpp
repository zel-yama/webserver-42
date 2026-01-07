


#include "../include/Server.hpp"
#include "../include/tools.hpp"

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
    char tmp[1024];
    int n  = 0;
 
    n = read(connect.fd, tmp, 1024);
    printf("this n of read byte in while {%d}\n", n);
    if (n < 0)
        return -1;
    if (n == 0)
        return 0;
  
    tmp[n] = '\0';
    connect.byteSent += n;
    std::string t = std::string(tmp, sizeof(tmp)); 
            printf("t- > {%s}\n", t.c_str());//new how to append is can be problem in missing byte 
    connect.buffer += tmp;

    
    return 1;

}

void readRequest(int fd,  Client &connect)
{
    //char tmp[4096];// read or sent  then by exist is problem may segfualt

    // int n  = recv(fd, tmp, sizeof(tmp), 0);
    if (myread(connect) == -1)
        return;
    // if (n <= 0 ){
    //     if (hasHeaderEnd(buffer)){
    //         if (checkPost(buffer))
    //         connect.requestFinish = true;
    //     }
    // }
    // if (connect.byteSent <= 0){
    //     return ; // client closed or error
    // }
    // tmp[n] = '\0';
    // printf("this is tmp >>>> |%s| \n", tmp);
    // connect.buffer += tmp;
    // connect.byteSent += n;
   
    printDebug(connect);
    if (hasHeaderEnd(connect.buffer)){
        std::cout  << "i has  he has header complite so don't  " << std::endl;
        if (hasHeaderBody(connect.buffer) && getContentLength(connect.buffer, connect)) {
            readBody(fd, connect);
        }
        if (!checkPost(connect.buffer))
            connect.requestFinish = true;
    }

}