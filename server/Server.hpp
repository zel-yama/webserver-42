
#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include <fstream>
using namespace std;

typedef struct ListenPort{
    unsigned int IPAddress;
    unsigned int Listport;
} AddressPort;
typedef struct Locations{
    string path;
    vector<string> directives;

} Locations;

class Server{

    private:
        AddressPort Address;
        string ServerName;
        string ClientMaxBody;
        vector<pair<int, string>> D_ErrorPages; //exit code with error page to that error 
        Locations located;
    public:
        void  parsing(string FileName);  
        void  FillLocation(vector<string>::iterator it ); 
        void  FillPort(string line );    



};








#endif