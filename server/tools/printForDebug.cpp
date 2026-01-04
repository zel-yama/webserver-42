
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
       
        printf("this is vetore value -> %s \n", it->c_str());
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
        printf("size locatin -> %lu\n", Serv.objLocation.size());
        printf("printf Atter \n");
        std::cout << "serv name -- > " <<  Serv.ServerName << std::endl;
        std::cout << "port -> "<< Serv.port << std::endl;
        std::cout << "ipaddress -> "<< Serv.ipAdress << std::endl;
       // std::cout << "autoindex -> " << Serv.outoIndex << std::endl;
        std::cout << "max byte size -> "<< Serv.bodyMaxByte << std::endl;
        std::cout << "root -> " << Serv.root << std::endl;
        printf("this is allowd methods \n");
        printvector(Serv.allowedMethods);
        printf("this index file \n");
        printvector(Serv.indexFile);
        printf("this is error page \n");
        printMapIntKey(Serv.D_ErrorPages);
}

void printKeyInt(IntKey v){
    IntKey::iterator it  = v.begin();
    while(it != v.end()){
        printf("this first -> %d and this second %s ", it->first, it->second.c_str());
        it++;
    }
}
void printlocation(location obj){
    
    printf("----------------- this is location path ------------ (%s) \n", obj.locationPath.c_str());
    printf("max size body -> %lu\n", obj.bodyMaxByte);
    printf("root  -> %s\n", obj.root.c_str());
    printf("this is allowed methods -> \n");
    printvector(obj.allowedMethods);
    printf("this error page \n");
    printMapIntKey(obj.D_ErrorPages);
    printf("end --------------------------------\n");
}

void printLocation(std::vector<location> objs){
    std::vector<location>::iterator it = objs.begin();
    while(it != objs.end()){
    
        printlocation(*it);
        it++;
    }
}


void printAllConfig(std::vector<Server> servs){
    
    printf(" size serves vetcor -> %lu\n", servs.size());

    std::vector<Server>::iterator it = servs.begin();

    int i = 0;

     while (it != servs.end()){

        // printf("printAllConfig\n");
        printAttr(*it);
        printLocation(it->objLocation);
        it++;  
        printf("-------------server-------------------\n");
     }
}
void printMapIntKey(std::map<int, std::string>  ma){

    mapIntKey::iterator it = ma.begin();
    while (it != ma.end())
    {
        printf("this key -> %d | and value -> %s \n", it->first, it->second.c_str());
        it++;
    }
    
}