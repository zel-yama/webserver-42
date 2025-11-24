

#include "Server.hpp"

vector<string> Server::split(string line, char delimiter){

    vector<string> v;
    string store;
    size_t pos = line.find(delimiter);
    size_t start = 0;
    while (pos != string::npos)
    {
        while(line[pos] == delimiter){
            pos++;
        }

    }
    v.push_back(line.substr(start));
    for(vector<string>::iterator it = v.begin(); it != v.end(); it++  ){
        cout << *it << endl;
    }
    
 
 
    return v;
}
`