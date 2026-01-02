
#include "../../webserv.hpp"
#include "../include/tools.hpp"
#include "../include/parsing.hpp"
#
void printdoulStr(doubleStr v){
    doubleIter it = v.begin();
    while(it != v.end()){
        std::cout << "|" << it->first << "|-> ";
        std::cout << "|" << it->second << "|" << std::endl;
        it++;
    }
}

void printvector(std::vector<std::string> &config){
    strIter it = config.begin();
    while(it != config.end()){
       
        std::cout << "|" << *it << "|\n" ;
        if (it->empty())
            std::cout << "i" << std::endl;
    
        it++;
    }
}

void printTocknes(std::vector<tockens> &TV){
    std::vector<tockens>::iterator it = TV.begin();
    while(it != TV.end()){
        std::cout << it->mytocken << "--> " << it->val  << std::endl;
        it++;
    }
}

void printAttr(Server &Serv){
        printf("printf Atter \n");
        std::cout << "serv name -- > " <<  Serv.ServerName << std::endl;
        std::cout << "port -> "<< Serv.port << std::endl;
        std::cout << "ipaddress -> "<< Serv.ipAdress << std::endl;
       // std::cout << "autoindex -> " << Serv.outoIndex << std::endl;
        std::cout << "max byte size -> "<< Serv.bodyMaxByte << std::endl;
        std::cout << "root -> " << Serv.root << std::endl;
        printKeyInt(Serv.D_ErrorPages);
        printvector(Serv.allowedMethods);
        printvector(Serv.indexFile);
}

void printKeyInt(IntKey v){
    IntKey::iterator it  = v.begin();
    while(it != v.end()){
        std::cout << "key -> " << it->first << std::endl;
        printf("value %s\n", it->second.c_str());
        it++;
    }
}

void printLocation(std::vector<location> objs){
    std::vector<location>::iterator it = objs.begin();
    while(it != objs.end()){
    
        printAttr(*it);
        it++;
    }
}


void printAllConfig(std::vector<Server> servs, Server serv){
    //std::vector<Server>::iterator it = servs.begin();
    int i = 0;
   
    printf("printAllConfig\n");
    printAttr(serv);
    printLocation(serv.objLocation);
     //it++;  
  

}