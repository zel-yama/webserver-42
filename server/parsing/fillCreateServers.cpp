

#include "../include/parsing.hpp"
#include "../include/tools.hpp"


location locationHandling(tockenIt &it){
    location local;
    
    while(it->mytocken != CLOSED_PRACKET){
     
        locationCases(it, local);
        it++;
    }
  return local;
}

void location_handle(Server &serv, std::string path)//extra 
 {
    serv.ServerName = path;
}

int serverHnding(tockenIt it, std::vector<Server> &servs){
    int i  = 0;
    location local;
    std::map<std::string, std::string> mapLoca;
    Server serv;
    while(it->mytocken != ENDSERV){
        if (!it->val.compare("location")){
            local = locationHandling(it);
            if (mapLoca.count(local.locationPath) == 0)
                mapLoca[local.locationPath] = local.locationPath;
            else    
                costumThrow("the locations has the same name ", "");
            serv.objLocation.push_back(local);
            if (serv.objLocation.size() > 100)
                costumThrow("many location", "");
        }
        else
            serverCases(it, serv);
        i++;
        it++;
    }
    
    if (serv.ipAdress.empty()|| serv.port == 0)
        myThrow();

    
    if (serv.bodyMaxByte == 0)
        serv.bodyMaxByte = 1e6;
    if (serv.ServerName.empty())
        serv.ServerName = "webServer";
    servs.push_back(serv);

    return i;
}

servers setUpServers(std::vector<tockens> &v){

    std::vector<Server> Servs;
    tockenIt it;
    maptype config;
    size_t i = 0;
    it = v.begin();
    while (it != v.end())
    {
      
      
        if (!it->val.compare("server")){
            i = serverHnding(it,Servs);
            if (i <= v.size())
                it += i;
            else    
                break;
        }
        it++;
         
    }
    if (Servs.size() > 200)
        costumThrow(" many servers", "In config File ");
    return Servs;
}