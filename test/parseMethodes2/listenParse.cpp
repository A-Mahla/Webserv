#include <string>
#include <iostream>
#include <cstdlib>
#include <ctype.h>
#include <locale>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include "Server.h"

#define MAX_PORT 65535
#define MIN_PORT 1024

int	checkOccurance(std::string & str, const char * toFind){
	int	i = 0;
	size_t occurs = 0;

	while (((occurs = str.find(toFind, occurs))) != std::string::npos){
		occurs++;
		i++;
	}
	return (i);
}

bool	checkSyntax(std::string str){
	if (checkOccurance(str, ";") != 1 || checkOccurance(str, ":") > 1 || checkOccurance(str, ".") > 3)
		return (false);
	return (true);
}

bool	onlyDiggit(std::string const & str){
	int i = 0;
	while (str[i]){
		if (str[i] > '9' || str[i] < '0'){
			return (false);
		}
		i++;
	}
	return (true);
}

bool	addrIsValid(std::string	addr){
	size_t	occurence = 0;
	int	pos = 0;

	while ( ( occurence = addr.find(".", occurence) ) != std::string::npos){
		if (!onlyDiggit(addr.substr(pos, occurence - pos)) || addr.substr(pos, occurence - pos).size() == 0)
		{
			return (false);
		}
		occurence++;
		pos = occurence;
	}
	if (!onlyDiggit(addr.substr(pos, occurence - pos)) || addr.substr(pos, occurence - pos).size() == 0)
	{
		return (false);
	}
	return (true);
}

void	resetDefault(Server & serv){
	serv = Server();
}

bool	addrIsGood(Server & serv, std::string addr){
	int		test = 0;
	int		res = 0;
	size_t	i = 0;

	if (!addr.compare("localhost") || !addr.compare("*")){
		serv.setAddr(INADDR_ANY);
		return (true);
	} else if (checkOccurance(addr, ".") == 3 && addrIsValid(addr)) {
		while (i < addr.size()){
			test = atoi(addr.c_str() + i);
			if (test < 0 || test > 255)
				return (false);
			res |= test;
			while (addr[i] && addr[i] != '.')
				i++;
			if (addr[i] == '.')
				res <<= 8;
			i++;
		}

		serv.setAddr(res);
		return (true);
	} else {
		return (false);
	}
}

bool	portIsGood(Server & serv, std::string port){
	int test = 0;
	if (!port.compare("default") || !port.compare("*")){
		serv.setPort(8080);
		return (true);
	}
	if ((test = atoi(port.c_str())) > MAX_PORT || (test = atoi(port.c_str())) < MIN_PORT)
		return (false);
	serv.setPort(test);
	return (true);
}

bool	listenParse(std::string str, Server & serv){

	if ( /*(serv._is_set["listen"] == true) &&*/ !(str.compare(0, 7, "listen ")) && checkSyntax((str.c_str() + 7) )){
		if (checkOccurance(str, ":") == 1){
			if ( addrIsGood(serv, str.substr(7, str.find(":", 0) - 7) ) \
			&& portIsGood(serv, str.substr((str.find(":", 0) + 1),  ( (str.find(";", 0)) - (str.find(":", 0) + 1) )) )){
			/*serv._is_set["listen"] = true;*/ // if both of my function are ok, listen is set, function return true
				return (true);
			}
		} else {
			if ( addrIsGood(serv, str.substr(7, str.find(";", 0) - 7)) ){
				serv.setPort(8080);
				/*serv._is_set["listen"] = true;*/ // only addr has been detected port is default 8080
				return (true);
			} else if ( portIsGood(serv, str.substr(7, str.find(";", 0) - 7)) ) {
				/*serv._is_set["listen"] = true;*/ // only port has been detected addr is default INADDR_ANY
				serv.setAddr(INADDR_ANY);
				return (true);
			} else {
				/*serv._is_set["listen"] = false;*/ // no port or addr detected, setting map to false, reseting default parametter and returning false
				resetDefault(serv);
				return (false);
			}
		}
	}
	/*serv._is_set["listen"] = false;*/ // no port or addr detected, setting map to false, reseting default parametter and returning false
	resetDefault(serv);
	return (false);
}
