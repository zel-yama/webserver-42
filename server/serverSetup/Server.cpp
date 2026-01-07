


#include "../include/Server.hpp"

Server::Server(){
    name = "Server";
    root = "./";
    bodyMaxByte = 1e8;
    ipAdress = "0.0.0.0";
    outoIndex = false;
    port = 8080;
    

}
location::location(){
    this->bodyMaxByte  = 0;
    this->outoIndex = false;
    
}

Client Server::acceptClient(){
    Client newOne;
    newOne.serverId = this->serverId;
    unsigned int len = sizeof(newOne.ClientSock);
    newOne.fd = accept(fd, reinterpret_cast<sockaddr *>(&newOne.ClientSock), &len);
    newOne.fd = makeNonBlockingFD(newOne.fd);
 
    if (newOne.fd < 0){
        ostringstream ss;
        ss << "server failed to accept connection from address " << inet_ntoa(newOne.ClientSock.sin_addr) << " Port : " << ntohs(newOne.ClientSock.sin_port);
        cerr << ss.str() << endl;
        exit(1);
    }
    newOne.data.events = EPOLLIN | EPOLLET;
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
    addressServer.sin_family = AF_INET;
    addressServer.sin_port = htons(port);
    addressServer.sin_addr.s_addr = inet_addr(ipaddress.c_str());
    if (bind(fd, reinterpret_cast<sockaddr *>(&addressServer),sizeof(addressServer)) < 0){
        throw runtime_error("error in bind operatoin bind function ");
    }
    listenFunction();
    data.events = EPOLLIN;
    data.data.fd = fd;
    return fd;
}