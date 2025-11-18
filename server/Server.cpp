

#include "Server.hpp"

void Server::FillPort(string line ){

    stringstream ss(line);
    string var;
    string ipaddress;
    ss >> var;
    ss >> ipaddress;
    int p =  ipaddress.find(":");
    var = ipaddress.substr(p + 1, 4);
    ipaddress = ipaddress.substr(0, p);
    ss.clear();
    ss.str("");
    ss << var;
    ss >> this->Address.Listport;
    this->Address.IPAddress = ipaddress;
}
string Server::getValue(string line, string &var){
    stringstream ss(line);

    string value;
    ss >> var;
    ss >> value;
    return value;
}
void Server::FillLocation(vector<string>::iterator it){
    
    string line = *it;
    string var;
    string value;
    value = getValue(line, this->located.path);
    
    it++;
    do {
        
        value = getValue(line, var);
        this->located.directives.insert( make_pair(var, value));
        it++;
        line = *it;
    }
    while(!strchr(line.c_str(), '}'));
    for(iter  it = this->located.directives.begin(); it != this->located.directives.end(); ++it){
        cout << it->first <<  "  "<< it->second <<  endl; 
    }

}
void Server::parsing(string FileName){
    
    
    vector<string> store;
    ifstream ObjFile(FileName.c_str());
    string line;
    while (getline(ObjFile, line))
    {
        store.push_back(line);
    }
    vector<string>::iterator it = store.begin();
    while (it != store.end())
    {
       // cout << *it << endl;
        line = *it;
        if (strstr(line.c_str(), "listen"))
            Server::FillPort(*it);
        if (strstr(line.c_str(),  "location"))
            Server::FillLocation(it);

        it++;
    }
    


}