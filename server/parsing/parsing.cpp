

#include "../include/parsing.hpp"

void handleComments(std::vector<std::string> &config){

    strIter it = config.begin();
    size_t pos;
    std::string str;
    while (it != config.end())
    {
        str = *it;
        if (str[0] == '#')
            config.erase(it);

        else if ( (pos = it->find("#")) != std::string::npos){
            *it = it->substr(0, pos);
        }
        it++;
    }
}

std::vector<std::string> stripSpaces(std::vector<std::string> &config){
    
    strIter it = config.begin();
    std::string str;
    std::vector<std::string> newvector;
    while (it != config.end()){
        
        str = removeSpaces(*it);
        if (!str.empty())
            newvector.push_back(str);
        
        it++;
    }
    return newvector;
}

bool checkBracket(std::string &s){

    for(size_t i = 0; i < s.size(); i++){
        if ((s[i] != '{' && s[i] != '}') && !std::isspace((unsigned char) s[i]))
        {
            return false;        
        } 

    }

    return true;
}

doubleStr convertVector( std::vector<std::string> &v){
    doubleStr newV;
    strIter it = v.begin();
    std::string node;
    size_t pos;
    while(it != v.end()){
        node = *it;
        
    
        if  (checkBracket(node)){
            newV.push_back(make_pair("", node));
            
        }
        else if ((pos = node.find(" ")) != std::string::npos){
            newV.push_back(std::make_pair(node.substr(0, pos), node.substr(pos)));  
        }
        else {
            newV.push_back(std::make_pair(node, ""));
        }
        it++;
    }
    return newV;
}
//////count bracket and end server marker 


/// cases {{}} {{aefifaenfawf ewaffef}} eaff}efaw " } " 
void pushTockens(TYPE tockensType, std::vector<tockens> &v, std::string s){

    tockens tock;
    tock.mytocken = tockensType;
    tock.val = s; 
    validatoinVarFormat(tock);

    if (!tock.val.empty())
        v.push_back(tock);

}
void parseFileExten(std::string &fileName){
    std::string exten = fileName.substr(fileName.size() - 5); 

    if (exten.compare(".conf"))
        throw std::runtime_error("Error: in file exstention " + fileName); //search on it
}

servers parsing(std::string FileName){
    
    
 
    servers servs;
    std::vector<std::string> store;
    doubleStr v;
    std::vector<tockens> tockV;
    std::ifstream ObjFile(FileName.c_str());
    std::string line;
    parseFileExten(FileName);
    while (getline(ObjFile, line))
    {
        if (!line.empty())
            store.push_back(line);
    }
    if (store.empty())
        throw std::runtime_error("Error:  < empty file or invalid > ");
    handleComments(store);
    store = stripSpaces(store);
    
    v = convertVector(store);
    
    tockV = tokeniezer(v);
    bracketValidtion(tockV);
   
    servs =  setUpServers(tockV);
    return servs;

}   