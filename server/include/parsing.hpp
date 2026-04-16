
#ifndef PARSING_HPP
#define PARSING_HPP


#include <iostream>
#include <vector>



#include "webserv.hpp"
#include "include.hpp"
#include "Server.hpp"


servers                     parsing(std::string FileName);
std::string                 removeSpaces(std::string &str);
std::vector<tockens>        tokenizer(doubleStr &v);
void                        pushTokens(TYPE tockensType, std::vector<tockens> &v, std::string s);
void                        methodsIntKey(std::map<int, std::string> &v, std::string str);
void                        methodsHandler(std::vector<std::string> &methdsV, std::string methods , int i);
void                        variableSingleValue(std::string str, std::string &buff);
void                        bodySizeMax(size_t &val, std::string &str);
int                         extractInt(std::string &s, std::string &c);
void                        insertListenConfig(Server &serv, std::string &str);
void                        autoIndexHandler(std::string val, int &cond);
servers                     setUpServers(std::vector<tockens> &v);
void                        validationVarFormat( tockens &mytockens);
int                         convertString(std::string &str);
void                        checkTokens(tockens &tockn);
void                        locationCases(tockenIt &it, location &local);
void                        serverCases(tockenIt &it, Server &serv);
void                        tockenValue(std::string &val, std::vector<tockens> &tockV, TYPE value);
void                        bracketValidation(std::vector<tockens> &tockens);
void                        returnP(std::string token, std::string &path, int &exitCode);
void                        handleMapString(std::string &str, std::map<std::string, std::string> &MapStrings);
std::vector<std::string>    splitV(std::string &str);

#endif