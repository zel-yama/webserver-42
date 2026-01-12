
#ifndef PARSING_HPP
#define PARSING_HPP


#include <iostream>
#include <vector>



#include "../../webserv.hpp"
#include "include.hpp"
#include "Server.hpp"


void                    printvector(std::vector<std::string> &config);
<<<<<<< HEAD
servers                parsing(std::string FileName);
=======
servers                 parsing(std::string FileName);
>>>>>>> origin
std::string             removeSpaces(std::string &str);
std::vector<tockens>    tokeniezer(doubleStr &v);
void                    pushTockens(TYPE tockensType, std::vector<tockens> &v, std::string s);
void                    methodsIntKey(std::map<int, std::string> &v, std::string str);
void                    methodesHandler(std::vector<std::string> &methdsV, std::string methods);
<<<<<<< HEAD
void                    rootHandler(std::string root, std::string &buff);
=======
void                    variableSingleValue(std::string str, std::string &buff);
>>>>>>> origin
void                    bodySizeMax(size_t &val, std::string &str);
int                     extractInt(std::string &s, std::string &c);
void                    insertListenConfig(Server &serv, std::string &str);
void                    outoIndexHandler(std::string val, bool &cond);
servers                  setUpServers(std::vector<tockens> &v);
void                    validatoinVarFormat( tockens &mytockens);
int                     convertString(std::string &str);
//--------------------------------------------------
    void printKeyInt(IntKey v);
    void printlocation(location obj);
    void printLocation(std::vector<location> objs);
    void printTocknes(std::vector<tockens> &TV);
    void printvector(std::vector<std::string> &config);
    void printMapIntKey(std::map<int, std::string>  ma);
#endif