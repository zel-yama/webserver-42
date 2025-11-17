

#include "Server.hpp"
void Server::FillPort(string line ){

    cout << "\n\n\n" << line  << " Fill port " << endl;
}
void Server::FillLocation(vector<string>::iterator it){
    cout << "\n\n" << *it << endl;
}
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
        line = *it;
        if ( line.compare("listen"))
            Server::FillPort(*it);
        if (line.compare("location"))

        it++;
    }
    


}