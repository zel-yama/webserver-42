

#include "../include/parsing.hpp"
#include "../include/tools.hpp"

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

int locationHandling(tockenIt it, Server &serv){
    location loca;
    int i = 0;
    while(it->mytocken != CLOSED_PRACKET){
        i++;
        printf("---------l--------------\n");
        serverCases(it, loca);
        it++;
    }
    serv.objLocation.push_back(loca);
    return i;
}
void location_handle(Server &serv, std::string path){
    serv.ServerName = path;
}



int serverHnding(tockenIt it, Server &serv){
    int i  = 0;
    location loca;
    while(it->mytocken != CLOSED_PRACKET ){
        if (!it->val.compare("location")){
            it++;
            location_handle(loca, it->val);
            it += locationHandling(it, loca);
        }
        else
            serverCases(it, serv);
        i++;
        it++;
    }
    serv.objLocation.push_back(loca);
    return i;
}
///////////////////// insert servers Vector in to config Map 
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
       // printf("----------------------------\n");
        if (!it->val.compare("server")){
            it += serverHnding(it, serv);
        }

        it++;
    }

    printAllConfig (Servs, serv);
    printf("-------sssss---------\n");
   // CreateServers(config, Servs);
    

}