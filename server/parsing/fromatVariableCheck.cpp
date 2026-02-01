/// the check var format and cause may variable and ever one have format he probaly check his format i should create for every varilble function to check to 
// 


#include "../include/parsing.hpp"
void resolveIpName(std::string &Str , Server &serv){
    
    if (Str.empty())
        return ;
    int status;
    struct addrinfo *result;
    struct  addrinfo data;
    data.ai_family = AF_INET;
    data.ai_socktype = SOCK_STREAM;
    // memset(result, 0, sizeof(result));
    // memset(&data, 0, sizeof(data));
    status = getaddrinfo(Str.c_str(), NULL, &data, &result);
    if (status)
        throw std::runtime_error("invalid host can't not be found [" + Str + "]");
    else
    {   
        struct addrinfo *tmp = result;
        while (tmp != NULL)
        {
            if (tmp->ai_family == AF_INET && tmp->ai_socktype == SOCK_STREAM){
                // void *ptr = ((sockaddr_in *)tmp->ai_addr)->sin_addr);
                serv.ipAdress =  tmp->ai_addr->sa_data;
            }
            tmp = tmp->ai_next;
        }
        

    }

    
    freeaddrinfo(result);
}
// no : means if str string ip address if number port check max 1 → 65535
void insertListenConfig(Server &serv, std::string &str){

    std::string por;
    int port;
    size_t pos = str.find(":");
    if (pos == std::string::npos)
    {
        if ((port = convertString(str)) == -1)
            resolveIpName(str, serv);
        else 
            serv.port = port;
    }
    else{
        serv.ipAdress = str.substr(0, pos);
        pos++;
        por = str.substr(pos);
        serv.port = convertString(por);

    }
    // std::cout << "port -> " << serv.port << "  iddress -> " << serv.ipAdress << std::endl ;
}
int extractInt(std::string &s, std::string &c){
    int number = -1;
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
    
    if (c.size() > 1 || max < 0)
        throw std::runtime_error("invalid value in max body size ");
    if (c[0] == 'M')
        max = max * 1e6;
    else if (c[0] == 'K')
        max = max * 1000;
    else if (c[0] == 'G')
        max = max * 1e9;
    // else 
    //     throw std::runtime_error("invalid value in max body size ");
    val = max;
}

void variableSingleValue(std::string str, std::string &buff){
    std::stringstream ss(str);
    std::string isEMpty;

    ss >> buff;
    ss >> isEMpty;
    if (!isEMpty.empty())
        throw std::runtime_error("invalid  extra value -> {" + str + "}");
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
void returnP(std::string token, std::string &path, int &exitCode){

    std::stringstream ss(token);

    std::string s;
    ss >> s;
    exitCode = convertString(s);
    if (exitCode == -1)
        costumThrow("invalid number of arguments", token);
    if (!ss.eof())
        ss >> path;
}

int convertString(std::string &str){
    std::stringstream ss(str);
    std::string s;
    int value;
    ss >> value;
    ss >> s;
    if (!s.empty() && !ss.eof())
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

<<<<<<< HEAD
void outoIndexHandler(std::string val, bool &cond){
    
    if (val.compare("on"))
        cond = true;
    else 
        cond = false;
=======
void outoIndexHandler(std::string val, int &cond){
    ///printf("auto index handle %s\n", val.c_str());

    if (!val.compare("on"))
        cond = 1;
    else 
        cond = 0;

>>>>>>> origin/master
}
