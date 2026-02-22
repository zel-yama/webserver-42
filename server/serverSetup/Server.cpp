


#include "../include/Server.hpp"
#include "../../request/RequestParser.hpp"
#include "../../Response/Response.hpp"

std::vector<int>  Config::fdsBuffer;
Server::Server(){
    this->cgiStatus = 0;
    this->upload = 0;
    this->outoIndex = 0;
    this->serverId = 0;
    this->returnCode = 0;
    bodyMaxByte = -1;
    
 
    outoIndex = false;
    port = -1;
  
    name = "Server";
    parser = new RequestParser();
    respone = new Response();

}
location::location(){
    this->outoIndex = -1;

    bodyMaxByte = -1;
    this->cgiStatus = -1;
    this->upload = -1;
    this->returnCode = 0;

   

  
    this->ex = false;
    
}
std::string convertIpAdder( uint32_t ipaddres){


    uint32_t ip =  ntohl(ipaddres); 

    std::stringstream ss;
    ss << ((ip >> 24) & 255) << '.' << ((ip >> 16) & 255) << '.' << ((ip >> 8) & 255) << "." << (ip & 255) ;   


    return ss.str();

}

Client Server::acceptClient(){
    Client newOne;
    newOne.serverId = this->fd; // i change this to fd?
    newOne.fdEp = fdEp;
    unsigned int len = sizeof(newOne.ClientSock);
    newOne.fd = accept(fd, reinterpret_cast<sockaddr *>(&newOne.ClientSock), &len);
    
    if (newOne.fd < 0){
        ostringstream ss;
        
        ss << "server failed to accept connection from address " << convertIpAdder(addressServer.sin_addr.s_addr) << " Port : " << ntohs(newOne.ClientSock.sin_port);
        cerr << ss.str() << endl;
        return newOne ;
    }
    newOne.fd = makeNonBlockingFD(newOne.fd);
    newOne.data.events = EPOLLIN | EPOLLHUP  | EPOLLERR;
    /// socket buffer input output 
    newOne.data.data.fd = newOne.fd;
    newOne.currentTime = time(NULL);
    fdsBuffer.push_back(newOne.fd);
    return newOne;
}
void Server::listenFunction(){
   
    if (listen(fd, 30) < 0){
       throw runtime_error("error in to listen that port ");}
    ostringstream ss;
    convertIpAdder(addressServer.sin_addr.s_addr);
    ss << "\n ** server IP address " << convertIpAdder(addressServer.sin_addr.s_addr) <<  " listening on port -> " << ntohs(addressServer.sin_port)  << " *** \n\n  " ; 
    cout << ss.str() << endl;
}
int Server::CreateServer(int port, string ipaddress){
    int opt = 1;
    fd =  socket(AF_INET, SOCK_STREAM, 0);
    fd = makeNonBlockingFD(fd);
    ipaddress = convertIpAdder(addressServer.sin_addr.s_addr);
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        throw runtime_error("Error: to set socket to reusing mode ");
    if (fd == -1)
    {
        throw runtime_error("error in in create socket socket function");
    }

    if (bind(fd, (sockaddr *)(&addressServer),sizeof(addressServer)) < 0){
        throw runtime_error("error in bind operatoin bind function ");
    }
    listenFunction();
    data.events = EPOLLIN;
    data.data.fd = fd;
    return fd;
}