
#ifndef PARSING_HPP
#define PARSING_HPP


#include <iostream>
#include <vector>



#include "../../webserv.hpp"
#include "include.hpp"
#include "Server.hpp"


void                    printvector(std::vector<std::string> &config);
void                    parsing(std::string FileName);
std::string             removeSpaces(std::string &str);
std::vector<tockens>    tokeniezer(doubleStr &v);
void                    pushTockens(TYPE tockensType, std::vector<tockens> &v, std::string s);
void                    methodsIntKey(IntKey &v, std::string str);
void                    methodesHandler(std::vector<std::string> &methdsV, std::string methods);
void                    rootHandler(std::string root, std::string &buff);
void                    bodySizeMax(size_t &val, std::string &str);
int                     extractInt(std::string &s, std::string &c);
void                    insertListenConfig(Server &serv, std::string &str);
void                    outoIndexHandler(std::string val, bool &cond);
void                    setUpServers(std::vector<tockens> &v, maptype &config);
//--------------------------------------------------
    void printKeyInt(IntKey v);
    void printLocation(std::vector<location> objs);
    void printTocknes(std::vector<tockens> &TV);
    void printvector(std::vector<std::string> &config);
#endif