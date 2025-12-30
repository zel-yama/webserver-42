
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
    CLOSED_PRACKET
}   TYPE;
typedef std::vector<std::string>::iterator strIter;
typedef std::vector < std::pair <std::string, std::string> > doubleStr;
typedef std::vector < std::pair < std::string, std::string> >::iterator doubleIter;
typedef struct tockens{
    TYPE mytocken;
    std::string val;
} tockens;
void printvector(std::vector<std::string> &config);
void parsing(std::string FileName);

#endif