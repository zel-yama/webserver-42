

#include "parsing.hpp"


std::string removeSpaces(std::string &str){

  
    size_t end = str.size() ;
    size_t start = 0;
    while(start < end && std::isspace((unsigned char)str[start]))
        start++;
    while(start < end && std::isspace((unsigned char)str[end-1]))
        end--;
    return str.substr(start, (end - start));
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

void validatoinVarFormat( tockens &mytockens){

    
    if (mytockens.mytocken == 0){
        if (mytockens.val[mytockens.val.size() -1] == ';')
            mytockens.val[mytockens.val.size()-1]  = ' ';
        else 
            throw std::runtime_error("this -> [" + mytockens.val + "] should be end up with ';'");
        removeSpaces(mytockens.val); 
    }
    if (mytockens.mytocken == 1){

    }

}