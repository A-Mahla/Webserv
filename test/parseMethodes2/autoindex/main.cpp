#include <iostream>
#include <string>
#include "Server.h"

bool	autoindexParse(std::string str, Server & serv);

int main(int ac, char **av){
	if (ac != 2){
		std::cout << "ERREUR ARGS !\n";
		return (0);
	}

	std::string test(*(av + 1));
	Server serv;


	std::cout << "***********TEST START************\n\n";
	int ret = autoindexParse(test, serv);
	std::cout << "[IN MAIN] return value is " << ret << "\n";
	std::cout << "\n***********TEST END************\n";


	std::cout << "\n\n***********PRINTING VALUE************\n\n";
	serv.print();

	return (0);
}

