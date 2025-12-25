
#ifndef PARSING_HPP
#define PARSING_HPP

using namespace std;
#include <iostream>

typedef enum TYPE{
    VALUE,
    VARIABLE,
    END, 
    OPENED_PRACKET,
    CLOSED_PRACKET
}   TYPE;

typedef struct tockens{
    TYPE mytocken;
    string val;
};



#endif