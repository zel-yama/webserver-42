
#include "parsing.hpp"
#include <fstream>
#include <sstream>

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
void printvector(std::vector<std::string> &config){
    strIter it = config.begin();
    while(it != config.end()){
       
        std::cout << "|" << *it << "|\n" ;
        if (it->empty())
            std::cout << "i" << std::endl;
    
        it++;
    }
}
std::string removeSpaces(std::string &str){

  
    size_t end = str.size() ;
    size_t start = 0;
    while(start < end && std::isspace((unsigned char)str[start]))
        start++;
    while(start < end && std::isspace((unsigned char)str[end-1]))
        end--;
    return str.substr(start, (end - start));
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
void printdoulStr(doubleStr v){
    doubleIter it = v.begin();
    while(it != v.end()){
        std::cout << "|" << it->first << "|-> ";
        std::cout << "|" << it->second << "|" << std::endl;
        it++;
    }
}
/// cases {{}} {{aefifaenfawf ewaffef}} eaff}efaw " } " 
void pushTockens(TYPE tockensType, std::vector<tockens> &v, std::string s){

    tockens tock;
    tock.mytocken = tockensType;
    tock.val = s; 
    v.push_back(tock);
}
void tockenValue(std::string &val, std::vector<tockens> &tockV){
    tockens tock;
    size_t i = 0;
    bool flagO = false;
    std::string buff  = "";
    bool flagC = false; 
    while(i < val.size()){
     
        if (val[i] == '{'){
                pushTockens(OPENED_PRACKET, tockV, "{");
                i++;
        }
        if (val[i] == '}'){
                pushTockens(CLOSED_PRACKET, tockV, "}");
                i++;
        }
       while(i < val.size())
        {
            if (val[i] != '}' && val[i] != '{')
            {
                buff.push_back(val[i]);
                i++;
            }
            else{
                if (!buff.empty()){
                    
                    buff = removeSpaces(buff);
                    if (!buff.empty())
                    {
    
                        pushTockens(VALUE, tockV, buff);
                        buff.clear();
                        break;
                    }
                    else 
                        break;
                }
            }    
        }
        if (!buff.empty())
            pushTockens(VALUE, tockV, buff);
    }
}
std::vector<tockens> tokeniezer(doubleStr &v){
    std::vector<tockens> tockenV;
    tockens tock;
    doubleIter it = v.begin();
    std::string var;
    std::string val;
    while(it != v.end()){
        var = it->first;
        val = it->second;
        if (!val.empty())
            pushTockens(VARIABLE, tockenV, var);
        if (it->second.empty())
            val = var;
        tockenValue(val, tockenV);
        it++;
    }
    return tockenV;
}
void printTocknes(std::vector<tockens> &TV){
    std::vector<tockens>::iterator it = TV.begin();
    while(it != TV.end()){
        std::cout << it->val << std::endl;
        it++;
    }
}

void parsing(std::string FileName){
    
    
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
    printf("--------here---------\n");
   tockV = tokeniezer(v);
    printTocknes(tockV);


}   