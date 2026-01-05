

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
    else if( !it->val.compare("location")){
        it++;
        rootHandler(it->val, serv.ServerName);
    }
 
}
void locationCases(tockenIt &it, location &local){
    
    
    if (!it->val.compare("root")){
        it++;
        rootHandler(it->val, local.root);
    }

    else if (!it->val.compare("client_max_body_size")){
        it++;
        bodySizeMax(local.bodyMaxByte, it->val);

    }
    else if (!it->val.compare("limit_except")){
        it++;
        methodesHandler(local.allowedMethods, it->val);
    }
    else if (!it->val.compare("autoindex")){
        it++;
        outoIndexHandler(it->val, local.outoIndex);
    }
    else if (!it->val.compare("index")){
        it++;
        methodesHandler(local.indexFile, it->val);
    }
 
    else if (!it->val.compare("error_page")){
        it++;
        methodsIntKey(local.D_ErrorPages, it->val);
    }
    else if( !it->val.compare("location")){
        it++;
        rootHandler(it->val, local.locationPath);
    }
 
}


location locationHandling(tockenIt &it){
    location local;
    
    while(it->mytocken != CLOSED_PRACKET){
     
        locationCases(it, local);
        it++;
    }
  return local;
}
void location_handle(Server &serv, std::string path){
    serv.ServerName = path;
}



int serverHnding(tockenIt it, std::vector<Server> &servs){
    int i  = 0;
    location local;
    Server serv;
    while(it->mytocken != ENDSERV){
        if (!it->val.compare("location")){
            local = locationHandling(it);
           
            serv.objLocation.push_back(local);

        }
        else
            serverCases(it, serv);
        i++;
        it++;
    }
    servs.push_back(serv);
    return i;
}
///////////////////// insert servers Vector in to config Map 
// maptype CreateServers( std::vector<Server> &serversV){
//     std::vector<Server>::iterator it = serversV.begin();
//     Server *serv;
//     maptype config;
//     Server Serv;
//     int fd = 0;
//     while(it != serversV.end()){
//         fd = it->CreateServer(it->port, it->ipAdress);
//         printf("fd -> %d\n", fd);
//         Serv = *it;
//         printf("port -> %d\n", Serv.port  );
//         config.insert(pair<int, Config *>(fd, &Serv));
//         it++;
//     }
  
//     return  config;

// }
///fill server and set init process statrt here in while 
servers setUpServers(std::vector<tockens> &v){

    std::vector<Server> Servs;
    tockenIt it;
    maptype config;
    size_t i = 0;
    it = v.begin();
    while (it != v.end())
    {
       // printf("----------------------------\n");
        if (!it->val.compare("server")){
            i = serverHnding(it,Servs);
            if (i <= v.size())
                it += i;
            else    
                break;
        }
        it++;
    }
    printf("------");
   printAllConfig (Servs);


    return Servs;
    

}