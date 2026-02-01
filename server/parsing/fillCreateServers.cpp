

#include "../include/parsing.hpp"
#include "../include/tools.hpp"

void serverCases(tockenIt &it, Server &serv){
    
    if (it->mytocken == CLOSED_PRACKET || it->mytocken == OPENED_PRACKET
        || it->mytocken == SERVER)
        return ;
    switch (it->mytocken)
    {
        case ROOT:
            it++;
            variableSingleValue(it->val, serv.root);
            break;
        case LISTEN: 
            it++;
            insertListenConfig(serv, it->val);
            break;
        case BODYMAX:
            it++;
            bodySizeMax(serv.bodyMaxByte, it->val); 
            break;   
        case METHODS:
            it++;
            methodesHandler(serv.allowedMethods, it->val);
            break;
        case AUTOINDEX:
            it++;
            outoIndexHandler(it->val, serv.outoIndex);
            break;
        case INDEX:
            it++;
            methodesHandler(serv.indexFile, it->val);
            break;
        case SERVERNAME:
            it++;
            variableSingleValue(it->val, serv.ServerName);
            break;
        case ERRORPAGE:
            it++;
            methodsIntKey(serv.D_ErrorPages, it->val);
            break;
        case RETURN:
            it++;
            returnP(it->val, serv.returnP, serv.returnCode);
            break;
        case CGI:
            it++;
            outoIndexHandler(it->val, serv.cgiStatus);
            break;
        case CGIEXTEN:
            it++;
            variableSingleValue(it->val, serv.cgiExten);
            break;
        case CGIPATH:
            it++;
            variableSingleValue(it->val, serv.cgiPath);
            break;
        case UPLOAD:
            it++;
            outoIndexHandler(it->val, serv.upload);
            break;
        case UPLOADPATH:
            it++;
            variableSingleValue(it->val, serv.uploadPath);
            break;
        default:
            throw std::runtime_error("invalid value {"+ it->val + "} put invalid config in server");
            break;
    }
}

void locationCases(tockenIt &it, location &local){
    
      if (it->mytocken == CLOSED_PRACKET || it->mytocken == OPENED_PRACKET)
        return ;
    switch (it->mytocken)
    {
        case ROOT:
            it++;
            variableSingleValue(it->val, local.root);
            break;
        case BODYMAX:
            it++;
            bodySizeMax(local.bodyMaxByte, it->val);
            break;
        case METHODS:
            it++;
            methodesHandler(local.allowedMethods, it->val);
            break;
        case AUTOINDEX:
            it++;
            outoIndexHandler(it->val, local.outoIndex);
            local.ex = true;
            break;
        case INDEX:
            it++;
            methodesHandler(local.indexFile, it->val);
            break;
        case ERRORPAGE:
            it++;
            methodsIntKey(local.D_ErrorPages, it->val);
            break;
        case LOCATION:
            it++;
            variableSingleValue(it->val, local.locationPath);
            break;
        case RETURN:
            it++;
            returnP(it->val,local.returnP, local.returnCode );
            break;
        case CGI:
            it++;
            outoIndexHandler(it->val, local.cgiStatus);
            break;
        case CGIEXTEN:
            it++;
            variableSingleValue(it->val, local.cgiExten);
            break;
        case CGIPATH:
            it++;
            variableSingleValue(it->val, local.cgiPath);
            break;
        case UPLOAD:
            it++;
            outoIndexHandler(it->val, local.upload);
            break;
        case UPLOADPATH:
            it++;
            variableSingleValue(it->val, local.uploadPath);
            break;
        default:
            throw std::runtime_error("invalid value {"+ it->val + "} put valid config in location");
            break;
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