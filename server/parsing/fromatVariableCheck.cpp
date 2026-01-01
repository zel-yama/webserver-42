/// the check var format and cause may variable and ever one have format he probaly check his format i should create for every varilble function to check to 
// 


#include "../include/parsing.hpp"

void insertListenConfig(Server &serv, std::string &str){

    size_t pos = str.find(":");
    serv.ipAdress = str.substr(0, pos);
    pos++;
    str =str.find(pos, str.size() - pos);
    serv.port = std::atoi(str.c_str());
    std::cout << "port -> " << serv.port << "iddress -> " << serv.ipAdress << std::endl ;

}
int extractInt(std::string &s, std::string &c){
    int number;
    std::stringstream ss(s);
    ss >> number;
    ss >> c;
    return (number);
}

void bodySizeMax(size_t &val, std::string &str){
    
    size_t max ;
    std::string c;
    max =  extractInt(str, c );
    if (c.size() > 1)
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

void rootHandler(std::string root, std::string &buff){
    buff = root;
}
/// now i handle limit methods like this methods get put after i handle {deny all}
void methodesHandler(std::vector<std::string> &methdsV, std::string methods){
    std::stringstream ss(methods);
    
    while(ss >> methods){
        methdsV.push_back(methods);
    }
}

void methodsIntKey(IntKey &v, std::string str){
    int key; 
    std::string value;
    std::stringstream ss(str);
    IntKey::iterator it;

    while(ss >> key){
        v.push_back(std::make_pair(key, value));
    }
    ss >> value;
    it = v.begin();
    while (it != v.end()){

        it->second = value;
        std::cout << "this is value -> " << value << "this is key -> " << it->first << std::endl;
        it++;
    }

}

void outoIndexHandler(std::string val, bool &cond){
    if (val.compare("on"))
        cond = true;
    else 
        cond = false;

}

