#include "Server.h"
#include <iostream>
#include <string>
#include <vector>

bool	serverListenConfig(std::string const & str, Server & current);

int main(int ac, char **av){
	(void) ac;
	Server s;
	std::string test( *(av + 1) );

	std::cout << "return value :" << serverListenConfig(test, s) << std::endl;

	std::cout << "Printing server values : \n";
	s.print();

	return (0);
}
