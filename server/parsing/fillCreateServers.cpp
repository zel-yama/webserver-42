

#include "../include/parsing.hpp"

void serverCases(tockenIt &it, Server &serv){
    if (!it->val.compare("root")){
        it++;
        rootHandler(it->val, serv.root);
    }
    else  if (!it->val.compare("listen")){
        it++;
        insertListenConfig(serv, it->val);
    
    }
    else if (!it->val.compare("client_max_body_size")){
        it++;
        bodySizeMax(serv.bodyMaxByte, it->val);

    }
    else if (!it->val.compare("limit_except")){
        it++;
        methodesHandler(serv.allowedMethods, it->val);
    }
    else if (!it->val.compare("autoindex")){
        it++;
        outoIndexHandler(it->val, serv.outoIndex);
    }
    else if (!it->val.compare("index")){
        it++;
        methodesHandler(serv.indexFile, it->val);
    }
    else if (!it->val.compare("server_name")){
        it++;
        rootHandler(it->val, serv.ServerName);
    }
    else if (!it->val.compare("error_page")){
        it++;
        methodsIntKey(serv.D_ErrorPages, it->val);
    }
}
void locationHandling(tockenIt &it, Server &serv){
    location loca;
    while(it->mytocken != CLOSED_PRACKET){
        serverCases(it, loca);
        it++;
    }
    serv.objLocation.push_back(loca);
}

Server serverHandler(tockenIt &it){
    Server serv;
    int i = 0;
    while(1){
        if (it->mytocken == OPENED_PRACKET){
            it++;
            i--;
        }
        else if (it->mytocken == CLOSED_PRACKET){    
            i++;
            it++;
        }
        if (i == 0)
            break;
        if (!it->val.compare("location"))
            locationHandling(it, serv);
        else 
            serverCases(it, serv);
        std::cout << "for debug ------------ \n" ;
    }
    return serv;
}
void CreateServers(maptype &config, std::vector<Server> &serversV){
    std::vector<Server>::iterator it = serversV.begin();
    Server Serv;
    int fd = 0;
    while(it != serversV.end()){
        fd = it->CreateServer(it->port, it->ipAdress);
        Serv = *it;
        config.insert(make_pair(fd, &Serv));
        it++;
    }

}

void setUpServers(std::vector<tockens> &v, maptype &config){

    std::vector<Server> Servs;
    tockenIt it;
    Server serv;
    int i = 0;
    it = v.begin();
    while (it != v.end())
    {
        if (!it->val.compare("server")){
            it++; 
            Servs.push_back(serverHandler(it));
        }
        else
            it++;
        std::cout << "------------------------\n" ; 
    }
   // CreateServers(config, Servs);
    

}