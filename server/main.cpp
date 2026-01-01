
#include "../webserv.hpp"
#include "./include/Server.hpp"
#include "include/parsing.hpp"
int main(int av, char *argc[]){

	Server Serv;
	maptype config; // parsing and fill this array create server and fill this  
	try{
		
		parsing(argc[1]);
		Serv.CreateServer(8081, "0.0.0.0");
		config.insert(pair<int, Config *>(Serv.fd, &Serv));
	
		// eventLoop(config);
	}
	catch (exception &e)
	{
		// cout << e.what() << endl;
		// close(Serv.fd);
		// return 1;
	}
	close(Serv.fd);
	return 0;
}
