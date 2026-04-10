#ifndef INCLUDE__HPP
#define INCLUDE__HPP 

#include "../../webserv.hpp"
#include "Config.hpp"


typedef enum TYPE{
    VALUE,
    VARIABLE,
    ENDSERV, 
    OPENED_PRACKET,
    CLOSED_PRACKET,
    SERVER, 
    LOCATION, 
    LOCATIONPATH,
    METHODS,
    BODYMAX,
    ERRORPAGE, 
    RETURN,
    ROOT,
    SERVERNAME,
    LISTEN,
    CGI, 
    AUTOINDEX,
    INDEX,
    UPLOADPATH,
    CGIPATH,
    UPLOAD,
}   TYPE;
typedef struct tockens{
    TYPE mytocken;
    std::string val;
} tockens;

typedef std::vector<std::pair<int, std::string> > IntKey;
typedef std::vector<tockens>::iterator tockenIt;
typedef std::vector<std::string>::iterator strIter;
typedef std::vector < std::pair <std::string, std::string> > doubleStr;
typedef std::vector < std::pair < std::string, std::string> >::iterator doubleIter;


typedef std::map<int,Config*>::iterator ConfigIter;
typedef std::map<int, Config*> maptype;
typedef std::map<int, std::string> mapIntKey;


#endif 

