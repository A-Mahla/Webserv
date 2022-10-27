#include <iostream>
#include <string>
#include "Server.h"

bool	listenParse(std::string str, Server & serv);

int main(int ac, char **av){
	if (ac != 2){
		std::cout << "ERREUR ARGS !\n";
		return (0);
	}

	std::string test(*(av + 1));
	Server serv;

	if (listenParse(test, serv))
		std::cout << "return value is TRUE" << std::endl;
	else
		std::cout << "return value is FALSE" << std::endl;
	std::cout << "*********printing settings***********\n";
	serv.print();
	return (0);
}

