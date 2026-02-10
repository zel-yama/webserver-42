


#include "../include/Server.hpp"
#include "../../request/RequestParser.hpp"
#include "../../Response/Response.hpp"

Server::Server(){
    this->cgiStatus = 0;
    this->upload = 0;
    this->outoIndex = 0;
    this->serverId = 0;
    this->returnCode = 0;
    infoFull = false;
    name = "Server";
    root = "./";
    bodyMaxByte = 1e8;
    ipAdress = "0.0.0.0";
    outoIndex = false;
    port = 8080;
    this->allowedMethods.push_back("GET");
    this->indexFile.push_back("index.html");
    parser = new RequestParser();
    respone = new Response();

}
location::location(){
    this->outoIndex = -1;

    this->bodyMaxByte = -1;
    this->cgiStatus = -1;
    this->upload = -1;
    this->returnCode = 0;
    this->allowedMethods.push_back("GET");
    this->indexFile.push_back("index.hmtl");
    this->bodyMaxByte  = 0;
    this->outoIndex = false;
    this->ex = false;
    
}

Client Server::acceptClient(){
    Client newOne;
    newOne.serverId = this->fd; // i change this to fd?
    unsigned int len = sizeof(newOne.ClientSock);
    newOne.fd = accept(fd, reinterpret_cast<sockaddr *>(&newOne.ClientSock), &len);
    newOne.fd = makeNonBlockingFD(newOne.fd);
 
    if (newOne.fd < 0){
        ostringstream ss;
        ss << "server failed to accept connection from address " << inet_ntoa(newOne.ClientSock.sin_addr) << " Port : " << ntohs(newOne.ClientSock.sin_port);
        cerr << ss.str() << endl;
        exit(1);
    }
    newOne.data.events = EPOLLIN | EPOLLHUP  | EPOLLERR;
    newOne.data.data.fd = newOne.fd;
    return newOne;
}
void Server::listenFunction(){
   
    if (listen(fd, 30) < 0){
       throw runtime_error("error in to listen that port ");}
    ostringstream ss;
    ss << "\n ** server IP address " << inet_ntoa(addressServer.sin_addr) <<  " listening on port -> " << ntohs(addressServer.sin_port)  << " *** \n\n  " ; 
    cout << ss.str() << endl;
}
int Server::CreateServer(int port, string ipaddress){
    int opt = 1;
    fd =  socket(AF_INET, SOCK_STREAM, 0);
    makeNonBlockingFD(fd);
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (fd == -1)
    {
        throw runtime_error("error in in create socket socket function");
    }
    if (!this->infoFull ){
        addressServer.sin_family = AF_INET;
        addressServer.sin_port = htons(port);
        addressServer.sin_addr.s_addr = inet_addr(ipaddress.c_str());

    }
    if (bind(fd, reinterpret_cast<sockaddr *>(&addressServer),sizeof(addressServer)) < 0){
        throw runtime_error("error in bind operatoin bind function ");
    }
    listenFunction();
    data.events = EPOLLIN;
    data.data.fd = fd;
    return fd;
}