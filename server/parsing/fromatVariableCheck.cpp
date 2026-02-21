/// the check var format and cause may variable and ever one have format he probaly check his format i should create for every varilble function to check to 
// 


#include "../include/parsing.hpp"

void HandleMapstrings(std::string &str, map<std::string, std::string> &MapStrings){
    std::vector<string> v = splitV(str);
    std::vector<std::string>::iterator it = v.begin();
    if (v.size() == 1 || v.size() > 2)
        throw std::runtime_error("Error in CGI path arguments " + str);
    while ((it + 1) != v.end())
    {
        MapStrings[*it]= *(it + 1);
        it++;
    }


}

void resolveIpName(std::string &Str , Server &serv){
    
    if (Str.empty())
        return ;
    int status;
    struct addrinfo *result;
    struct  addrinfo data;
    data.ai_family = AF_INET;
    data.ai_socktype = SOCK_STREAM;
    memset(&data, 0, sizeof(data));
    status = getaddrinfo(Str.c_str(), NULL, &data, &result);
    if (status)
        throw std::runtime_error("invalid host can't not be found [" + Str + "]");
    else
    {   
        struct addrinfo *tmp = result;
        while (tmp != NULL)
        {
            if (tmp->ai_family == AF_INET && tmp->ai_socktype == SOCK_STREAM){
             
                 
                
                serv.addressServer.sin_family = tmp->ai_family;
                memcpy(&serv.addressServer.sin_addr, &tmp->ai_addr->sa_data[2], 4);
                serv.addressServer.sin_port = htons(serv.port);
            
     
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
    if (!serv.ipAdress.empty() || str.find(" ") != std::string::npos)
        costumThrow("", str);
    if (pos == std::string::npos)
    {
        if ((port = convertString(str)) == -1)
            myThrow();
        else {

            serv.port = port;
            serv.ipAdress = "localhost";
            resolveIpName(serv.ipAdress, serv);
        }
    }
    else{
        serv.ipAdress = str.substr(0, pos);
        pos++;
        por = str.substr(pos);
        serv.port = convertString(por);
        resolveIpName(serv.ipAdress, serv);
    }
    if (serv.port > 65535)
       throw  std::runtime_error("invalid port max is [65535..!]");    
}
int extractInt(std::string &s, std::string &c){
    int number = -1;
    std::stringstream ss(s);
    ss >> number;
    ss >> c;
    return (number);
}

void bodySizeMax(size_t &val, std::string &str){
    
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
    else if (!c.empty() || val != -1) 
        throw std::runtime_error("invalid value in max body size ");
    val = max;
}

void variableSingleValue(std::string str, std::string &buff){
    std::stringstream ss(str);
    std::string isEMpty;
    std::string tmp = buff;

    ss >> buff;
    ss >> isEMpty;
    if (!isEMpty.empty() || !tmp.empty())
        throw std::runtime_error("Error: Invalid token near   -> {" + str + "}");
}
/// count of methods should 3 or less without no doblecate 
void methodesHandler(std::vector<std::string> &methdsV, std::string methods , int i){
    std::stringstream ss(methods);

    if (!methdsV.empty())
        myThrow();
    while(ss >> methods){
        if ((!methods.compare("GET") && !methods.compare("POST") && !methods.compare("DELETE")) && i == 1)
            throw std::runtime_error("Error: Invalid token near  -> " + methods);
        if ((i = 1 && methdsV.size() > 3) )
            myThrow();
       
        methdsV.push_back(methods);
    }
    if (i == 1 && methdsV.size() > 1){
        if (methdsV.size() == 2 && (methdsV[0] == methdsV[1]))
            myThrow();
        else if (methdsV.size() == 3 && (methdsV[0] == methdsV[1] || methdsV[0] == methdsV[2] || methdsV[2] == methdsV[1] ))
            myThrow();
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

    if (exitCode != 0 || !path.empty())
        throw std::runtime_error("Error: Invalid token near < "+ token +" >");
    std::string s;
    ss >> s;
    exitCode = convertString(s);
    if (exitCode == -1 || (exitCode < 300 || exitCode > 599))
        costumThrow("invalid number of arguments", token);
    if (!ss.eof())
        ss >> path;
}

int convertString(std::string &str){
    std::stringstream ss(str);
    std::string s;
    int value = -1;
    ss >> value;
    ss >> s;
    if (!s.empty() || !ss.eof())

    if (!s.empty() || !ss.eof())
        return -1;
  
    return value;
}

void methodsIntKey(std::map<int, std::string> &v, std::string str){
    
    int key; 
    std::string value;
    std::vector<std::string> vS;
    vS = splitV(str);
    strIter its = vS.begin();
    if (convertString(vS.back() ) == -1)
        value = vS.back();
  
    while((its  + 1)  != vS.end()){

        key = convertString(*its);
        if (key == -1 || (key <= 299 || key > 599 ))
            throw std::runtime_error("Error: Invalid token near > " + str);
        its++;
        v.insert(make_pair(key, value));
    }

    if (value.empty())
       throw std::runtime_error("Error: invalid value [" + value + "] in error page");
}

void outoIndexHandler(std::string val, int &cond){
    
    if (cond == 1)
        throw std::runtime_error("Error: Invalid token near \'" + val + "\'");
    if (!val.compare("on"))
        cond = 1;
    else 
        cond = 0;
}
