
#include "../webserv.hpp"
#include "./include/Server.hpp"
#include "include/parsing.hpp"
int main(int av, char *argc[]){

	Server *Serv;
	maptype config;
	int fd;
	servers serv ;
	servers::iterator it;
	signal(SIGPIPE, SIG_IGN);
	if (av != 2 ){
		std::cerr << "invalid argument we should take [tow..!] " << std::endl;
		return 1;
	}

	try{
		
		serv = parsing(argc[1]);
		it = serv.begin();
		while(it != serv.end()){
			Serv = new Server(*it);
			fd = Serv->CreateServer( Serv->ipAdress);
			
			Serv->serverId = fd;
			Serv->fd = fd;
			config[fd] = Serv;
			it++;
		}
		eventLoop(config);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		
		cleanUP(config);
	}

	return 0;
}
