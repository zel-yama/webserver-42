
#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <cstring>
#include <vector>
#include <map>
using namespace std;

typedef struct ListenPort{
    unsigned int IPAddress;
    unsigned int Listport;
} AddressPort;
typedef struct Locations{


};
typedef struct Directives{

};

class Server{

    private:
        AddressPort Address;
        string ServerName;
        string ClientMaxBody;
        vector<int, string> D_ErrorPages; //exit code with error page to that error 
        // string *AllowMethods;
        // bool    AutoIndex;



};








#endif