
#include "Server.hpp"

int main(int av, char *argc[]){

    Server Serv;
    map<int, Config> config; // parsing and fill this array create server and fill this  
    Serv.CreateServer(8080, "0.0.0.0");
    eventLoop( Serv);

}