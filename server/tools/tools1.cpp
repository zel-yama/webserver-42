

#include "../include/tools.hpp"


void    costumThrow(std::string des, std::string invalid){


    
    throw std::runtime_error("Error: Invalid token near  + -> [" + invalid + " ] " + des);
}

void myThrow(){
    throw std::runtime_error("ParserError: Invalid");
}

