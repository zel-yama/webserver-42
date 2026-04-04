

#include "../../webserv.hpp"
#include "../include/tools.hpp"


void    costumThrow(std::string des, std::string invalid){


    
    throw std::runtime_error("Error: Invalid token near  + -> [" + invalid + " ] " + des);
}

void myThrow(){
    throw std::runtime_error("ParserError: Invalid");
}

void printStrings(std::string str1, std::string str2, std::string str3, int str4){
    
   __displayTime() ;
    std::cout <<  " : " <<  str1 << " : " << str2 << " : "  << str3 << " : " << str4 << " : " << std::endl; 
}