#pragma once


// for includ's and prototyp's
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
#include <sys/epoll.h>
#include "./server/Config.hpp"
#include "./server/Client.hpp"
 #include <stdlib.h>
#include <cerrno>
#include "fcntl.h"
#include <unistd.h>
#include <cerrno>
#include <stdio.h>
using namespace std;
typedef map<string, string>::iterator iter;
typedef map<int,Config*>::iterator ConfigIter;
typedef map<int, Config*> maptype;



