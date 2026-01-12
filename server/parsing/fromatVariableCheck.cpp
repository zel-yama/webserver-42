/// the check var format and cause may variable and ever one have format he probaly check his format i should create for every varilble function to check to 
// 


#include "../include/parsing.hpp"

void insertListenConfig(Server &serv, std::string &str){

    std::string por;
    size_t pos = str.find(":");
    // if (pos != std::string::npos)
    //     throw std::runtime_error("Error format of config address and port ");
    serv.ipAdress = str.substr(0, pos);
    pos++;
    por = str.substr(pos);
    serv.port = convertString(por);
    // std::cout << "port -> " << serv.port << "  iddress -> " << serv.ipAdress << std::endl ;
}
int extractInt(std::string &s, std::string &c){
<<<<<<< HEAD
    int number;
=======
    int number = -1;
>>>>>>> origin
    std::stringstream ss(s);
    ss >> number;
    ss >> c;
    return (number);
}

void bodySizeMax(size_t &val, std::string &str){
    
    ///printf("body size -> %s\n", str.c_str());
    size_t max ;
    std::string c;
    max =  extractInt(str, c );
<<<<<<< HEAD
    if (c.size() > 1)
=======
    
    if (c.size() > 1 || max < 0)
>>>>>>> origin
        throw std::runtime_error("invalid value in max body size ");
    if (c[0] == 'M')
        max = max * 1e6;
    else if (c[0] == 'K')
        max = max * 1000;
    else if (c[0] == 'G')
        max = max * 1e9;
    else 
        throw std::runtime_error("invalid value in max body size ");
    val = max;
}

<<<<<<< HEAD
void rootHandler(std::string root, std::string &buff){
    // printf("rooot handl %s\n", root.c_str());
    buff = root;
=======
void variableSingleValue(std::string str, std::string &buff){
    std::stringstream ss(str);
    std::string isEMpty;

    ss >> buff;
    ss >> isEMpty;
    if (!isEMpty.empty())
        throw std::runtime_error("invald  excrat value{" + str + "}");
>>>>>>> origin
}
/// now i handle limit methods like this methods get put after i handle {deny all}
void methodesHandler(std::vector<std::string> &methdsV, std::string methods){
    std::stringstream ss(methods);
    // printf("methods handle %s\n", methods.c_str());
    while(ss >> methods){
        methdsV.push_back(methods);
    }
}

std::vector<std::string> splitV(std::string &str){

    std::stringstream ss(str);
    std::string store;
    std::vector<std::string> v;
    while (ss >> store)
    {
        v.push_back(store);     
    }
    return v;
}

int convertString(std::string &str){
    std::stringstream ss(str);
    int value;
    ss >> value;
    if (!ss.eof())
        return -1;
    else 
        return value;
}

// resolve error page 505 504 7848 /eror.html
void methodsIntKey(std::map<int, std::string> &v, std::string str){
    
    int key; 
    std::string value;
    std::vector<std::string> vS;
    vS = splitV(str);
    strIter its = vS.begin();
    if (convertString(vS.back()) == -1)
        value = vS.back();
  
    while(its != vS.end()){
        
        key = convertString(*its);
        if (key == -1)
            break;
        its++;
        v.insert(make_pair(key, value));
    }
    if (its != vS.end() && value.empty())
       throw std::runtime_error("invalid value [" + *its + "]in error page");

}

void outoIndexHandler(std::string val, bool &cond){
    ///printf("auto index handle %s\n", val.c_str());
    if (val.compare("on"))
        cond = true;
    else 
        cond = false;

}

