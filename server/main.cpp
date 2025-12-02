
#include "Server.hpp"

int main(int av, char *argc[]){

    Server Serv;
    maptype config; // parsing and fill this array create server and fill this  
    Serv.CreateServer(150, "0.0.0.0");
    config.insert(pair<int, Config *>(Serv.fd, &Serv));

    eventLoop( config);

}
