
#include "Server.hpp"

int main(int av, char *argc[]){

    Server Serv;
    if ( av != 2)
    {
        cerr << "invalid input arguments " << endl;
        return 1;
    }
    Serv.parsing(argc[1]);


}