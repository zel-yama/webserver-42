

#include "Server.hpp"
#include "Client.hpp"
void Server::FillPort(string line ){

    stringstream ss(line);
    string var;
    string ipaddress;
    ss >> var;
    ss >> ipaddress;
    int p =  ipaddress.find(":");
    var = ipaddress.substr(p + 1, 4);
    ipaddress = ipaddress.substr(0, p);
    ss.clear();
    ss.str("");
    ss << var;
    ss >> this->Address.Listport;
    this->Address.IPAddress = ipaddress;
}
Server::Server(){
    name = "Server";
}
string Server::getValue(string line, string &var ){
    
    for (int i = 0; i < line.size(); i++){
        if (line[i] == '{' || line[i] == '}' || line[i] == ';')
            line[i] = ' ';
    }
    stringstream ss(line);
    string value;
    ss >> var;
    ss >> value;
    return value;
}
void Server::FillLocation(vector<string>::iterator it){
    
    string line = *it;
    string var;
    string value;
    getValue(line, this->located.path);
    do {  
        it++;
        line = *it;
        value = getValue(line, var);
        this->located.directives.insert( make_pair(var, value));
    }
    while(!strchr(line.c_str(), '}'));
}
void Server::getMethods(string &line){

    
}
void Server::parsing(string FileName){
    
    
    vector<string> store;
    string var;
    ifstream ObjFile(FileName.c_str());
    string line;
    while (getline(ObjFile, line))
    {
        store.push_back(line);
    }
    vector<string>::iterator it = store.begin();
    while (it != store.end())
    {
       // cout << *it << endl;
        line = *it;
        if (strstr(line.c_str(), "listen"))
            Server::FillPort(*it);
        if (strstr(line.c_str(),  "location"))
            Server::FillLocation(it);
        if (strstr(line.c_str(), "methods"))
           // split(line, " ");
        it++;
    }
}
Client Server::acceptClient(){
    Client newOne;
    newOne.name = "client";
    unsigned int len = sizeof(newOne.ClientSock);
    newOne.fd = accept(fd, reinterpret_cast<sockaddr *>(&newOne.ClientSock), &len);
    newOne.fd = makeNonBlockingFD(newOne.fd);
    if (newOne.fd < 0){
        ostringstream ss;
        ss << "server failed to accept connection from address " << inet_ntoa(newOne.ClientSock.sin_addr) << " Port : " << ntohs(newOne.ClientSock.sin_port);
        cerr << ss.str() << endl;
        exit(1);
    }
    newOne.data.events = EPOLLIN;
    newOne.data.data.fd = newOne.fd;
    return newOne;
}
void Server::listenFunction(){
   
    if (listen(fd, 30) < 0){
        cerr << "error in listen operation " << endl;
        return ;
    }
    ostringstream ss;
    ss << "\n ** server IP address " << inet_ntoa(addressServer.sin_addr) <<  " listening on port -> " << ntohs(addressServer.sin_port)  << " *** \n\n  " ; 
    cout << ss.str() << endl;
}
int Server::CreateServer(int port, string ipaddress){
    fd =  socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        cerr << " error in socket creating " << endl;
        return -1;
    }
    addressServer.sin_family = AF_INET;
    addressServer.sin_port = htons(port);
    addressServer.sin_addr.s_addr = inet_addr(ipaddress.c_str());
    if (bind(fd, reinterpret_cast<sockaddr *>(&addressServer),sizeof(addressServer)) < 0){
        cerr << "error in bind operation " << endl;
        return 1;
    }
    listenFunction();
    data.events = EPOLLIN;
    data.data.fd = fd;
    return fd;
}