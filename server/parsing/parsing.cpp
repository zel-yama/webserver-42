

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
void bracketValidtion(std::vector<tockens> &tockens){
    int count_closed = 0;
    int count_opened = 0;
    tockenIt it = tockens.begin();
    while(it != tockens.end()){
        if (it->mytocken == OPENED_PRACKET)
            count_opened++;
        if (it->mytocken == CLOSED_PRACKET)
            count_closed++;
        it++;
    }  
    if ((count_closed != count_opened) || count_closed == 0 || count_opened == 0)
        throw std::runtime_error("Error in bracket "); 
    count_closed = 0;
    count_opened = 0;
    it = tockens.begin();
    while (it != tockens.end())
    {
        if (!it->val.compare("server")){
            it++;
            while (it != tockens.end())
            {
                
                if (it->mytocken == OPENED_PRACKET)
                    count_opened++;
                if (it->mytocken == CLOSED_PRACKET)
                    count_closed++;
                if (count_closed == count_opened){
                    count_closed = 0;
                    count_opened = 0;
                    it->mytocken = ENDSERV;
                    break;
                }
                it++;
            }
        }
        it++;
    }
    
}

/// cases {{}} {{aefifaenfawf ewaffef}} eaff}efaw " } " 
void pushTockens(TYPE tockensType, std::vector<tockens> &v, std::string s){

    tockens tock;
    tock.mytocken = tockensType;
    tock.val = s; 
    validatoinVarFormat(tock);

    if (!tock.val.empty())
        v.push_back(tock);

}

servers parsing(std::string FileName){
    
    
    maptype config;
    servers servs;
    std::vector<std::string> store;
    //std::string var;
    doubleStr v;
    std::vector<tockens> tockV;
    std::ifstream ObjFile(FileName.c_str());
    std::string line;
    while (getline(ObjFile, line))
    {
        if (!line.empty())
            store.push_back(line);
    }
    handleComments(store);
    store = stripSpaces(store);
    //printvector(store);
    v = convertVector(store);
    //printdoulStr(v);
    tockV = tokeniezer(v);
    ///i should create fucntion to count brackets and mark up end of servers 
    bracketValidtion(tockV);
    printTocknes(tockV);
    ///servs =  setUpServers(tockV);
    return servs;

}   