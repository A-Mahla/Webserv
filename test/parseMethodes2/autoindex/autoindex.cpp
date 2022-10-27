#include <string>
#include <iostream>
#include <cstdlib>
#include <ctype.h>
#include <locale>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include "Server.h"

			/*  AUTOINDEX PARSING  */
			/*
			 * autoindex allow server to response a listing of file and directories available when the request do not end reaching a file
			 * for exemple, when the url end with "http://.../toto/" and the autoindex is on, then a listing of file and directories inside toto/ will be send
			 *
			 *
			 * ALLOWED SYNTAX:
			 *
			 * autoindex on;
			 * autoindex off;
			 *
			 * by default, autoindex is set with 'off' attribute
			*/

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
	int i = 0;

	std::cout << "[IN CHECK SYNTAX] : [" << str << "]" << std::endl;
	if (checkOccurance(str, ";") != 1)
		return (false);
	while (str[i] != ';')
		i++;
	i++;
	while (str[i]){
		if (str[i] != ' ' && str[i] != '\t')
			return (false);
		i++;
	}
	return (true);
}

bool	autoindexParse(std::string str, Server & serv){
	if ( /*(serv._is_set["autoindex"] == false) &&*/ !( str.compare(0, 10, "autoindex ") ) && checkSyntax( (str.c_str() + 10) )) {
		if ( !str.compare(10, 4, "off;") ) {
			/*serv._is_set["autoindex"] == true;*/
			serv.setAutoindex(false);
			return (true);
		} else if (!str.compare(10, 3, "on;") ) {
			/*serv._is_set["autoindex"] == true;*/
			serv.setAutoindex(true);
			return (true);
		} else
			return (false);
	}
	return (false);
}
