
#ifndef PARSING_HPP
#define PARSING_HPP


#include <iostream>
#include <vector>
#include <string>
#include <cctype>


typedef enum TYPE{
    VALUE,
    VARIABLE,
    END, 
    OPENED_PRACKET,
    CLOSED_PRACKET,
    SERVER, 
    LOCATION, 
    METHODS
}   TYPE;
typedef std::vector<std::string>::iterator strIter;
typedef std::vector < std::pair <std::string, std::string> > doubleStr;
typedef std::vector < std::pair < std::string, std::string> >::iterator doubleIter;
typedef struct tockens{
    TYPE mytocken;
   // std::string var[14] = {"cgi","limit_except", "root", "listen", "client_max_body_size", "error_page", "location", "server", "server_name", "index", "return", "autoindex"};
    std::string val;
} tockens;
void printvector(std::vector<std::string> &config);
void parsing(std::string FileName);
std::string removeSpaces(std::string &str);
std::vector<tockens> tokeniezer(doubleStr &v);
void pushTockens(TYPE tockensType, std::vector<tockens> &v, std::string s);
#endif