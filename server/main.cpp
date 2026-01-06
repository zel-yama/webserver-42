
#include "../webserv.hpp"
#include "./include/Server.hpp"
#include "include/parsing.hpp"
int main(int av, char *argc[]){

	Server Serv;
	maptype config;
	int fd;
	servers serv ;
	servers::iterator it;
	if (av != 2 ){
		std::cerr << "invalid argument we should take [tow..!] " << std::endl;
		return 1;
	}
	// parsing and fill this array create server and fill this  
	try{
		
		serv = parsing(argc[1]);
		it = serv.begin();
		while(it != serv.end()){
			Serv = *it;
			
			fd = Serv.CreateServer(Serv.port, Serv.ipAdress);
			config.insert(pair<int, Config *>(fd, &Serv));
			it++;
		}
		
		//eventLoop(config);
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
