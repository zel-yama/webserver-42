
#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <cstring>
#include <sys/types.h>         
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;
typedef map<string, string>::iterator iter;

typedef struct ListenPort{
    string      IPAddress;
    unsigned int Listport;
} AddressPort;
typedef struct Locations{
    string path;
    map<string,string> directives;
} Locations;

class Server{

    private:
        AddressPort Address;
        int fd;
        struct sockaddr_in addressServer;
        string ServerName;
        string ClientMaxBody;
        vector<string> AllowMethods; 
        vector<pair<int, string > > D_ErrorPages; //exit code with error page to that error 
        Locations located;
        string getValue(string line, string &var);
        void  getMethods(string &line );   
        void  FillPort(string line ); 
        void  FillLocation(vector<string>::iterator it ); 
    public:
        void  parsing(string FileName);  
        vector<string>  split(string line, char delimiter);
        int  CreateServer(int port, string ipaddress );
        void listenFunction();
        Client acceptClient();


};








#endif