
#include "../webserv.hpp"
#include "./include/Server.hpp"
#include "include/parsing.hpp"
int main(int av, char *argc[]){

	Server Serv;
	maptype config;
	int fd;
	servers serv ;
	servers::iterator it;
<<<<<<< HEAD
=======
	if (av != 2 ){
		std::cerr << "invalid argument we should take [tow..!] " << std::endl;
		return 1;
	}
>>>>>>> origin
	// parsing and fill this array create server and fill this  
	try{
		
		serv = parsing(argc[1]);
		it = serv.begin();
		while(it != serv.end()){
			Serv = *it;
<<<<<<< HEAD
			
			fd = Serv.CreateServer(Serv.port, Serv.ipAdress);
=======
			fd = Serv.CreateServer(Serv.port, Serv.ipAdress);
			Serv.serverId = fd;
>>>>>>> origin
			config.insert(pair<int, Config *>(fd, &Serv));
			it++;
		}
		
<<<<<<< HEAD
<<<<<<< HEAD
		eventLoop(config);
=======
		//eventLoop(config);
>>>>>>> origin
=======
		eventLoop(config);
>>>>>>> 1b3052310a3c050f0c43e700afad5866ff9f4475
	}
	catch (exception &e)
	{
		cout << e.what() << endl;
		close(Serv.fd);
		return 1;
	}
	close(Serv.fd);
	return 0;
}
