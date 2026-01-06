#ifndef INCLUDE__HPP
#define INCLUDE__HPP 

#include "../../webserv.hpp"
#include "Config.hpp"
using namespace std;

typedef enum TYPE{
    VALUE,
    VARIABLE,
    ENDSERV, 
    OPENED_PRACKET,
    CLOSED_PRACKET,
    SERVER, 
    LOCATION, 
    METHODS,
    LOCATIONPATH,
}   TYPE;
typedef struct tockens{
    TYPE mytocken;
   // std::string var[14] = {"cgi","limit_except", "root", "listen", "client_max_body_size", "error_page", "location", "server", "server_name", "index", "return", "autoindex"};
    std::string val;
} tockens;

typedef std::vector<std::pair<int, std::string> > IntKey;
typedef std::vector<tockens>::iterator tockenIt;
typedef std::vector<std::string>::iterator strIter;
typedef std::vector < std::pair <std::string, std::string> > doubleStr;
typedef std::vector < std::pair < std::string, std::string> >::iterator doubleIter;


typedef map<int,Config*>::iterator ConfigIter;
typedef map<int, Config*> maptype;
typedef map<int, std::string> mapIntKey;


#endif 

