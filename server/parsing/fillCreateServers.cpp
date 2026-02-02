

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