

#include "Server.hpp"


void Server::parsing(string FileName){
    
    
    vector<string> store;
    ifstream ObjFile(FileName);
    string line;
    while (getline(ObjFile, line))
    {
        store.push_back(line);
    }
    vector<string>::iterator it = store.begin();
    while (it != store.end())
    {
        cout << *it << endl;
        it++;
    }
    


}