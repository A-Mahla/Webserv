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



// keyword must be listen
// only port and Ipv4 addr are accepted
// they must be separated by ':'
// line finish by ';'

// **DEFAULT VALUE ARE "INADDR_ANY : 8080**"

int	isPort( std::string occurs, Server & current){
	int	res = 0;
	int i = 0;
	if (occurs.compare(0, 8, "default;") == 0 || occurs.compare(0, 2, "*;") == 0){
		current.setPort(8080);
		return (true);
	}
	for (; occurs[i] != ';' ; i++){
		if (occurs[i] > '9' || occurs[i] < '0')
			return (false);
	}

	res = atoi(occurs.c_str());
	if (res < MIN_PORT || res  > MAX_PORT)
		return (false);
	current.setPort(res);
	return (true);
}

int	isaddr( std::string occurs, Server & current) {
	int res = 0;
	int test = 0;
	int dot_ct = 0;

	if (occurs.compare(0, 9, "localhost") == 0 || occurs.compare(0, 1, "*") == 0)
	{
		current.setAddr(INADDR_ANY);
		return (true);
	}
	for (int i = 0; occurs[i] != ';' && occurs[i] != ':' && occurs[i]; i++)
	{
		test = atoi((occurs.c_str() + i));
		if (test < 0 || test > 255)
		{
			return (false);
		}
		while ( occurs[i] >= '0' && occurs[i] <= '9' ){
			i++;
		}
		if (occurs[i] != '.' && occurs[i] != ';' && occurs[i] != ':'){

			return (false);
		}
		else if (occurs[i] == '.')
			dot_ct++;

		res |= test;
		if (occurs[i] != ';' && occurs[i] != ':')
			res = (res << 8);
		if (occurs[i] == ';' || occurs[i] == ':')
			break;
	}
	if (dot_ct == 3){
		current.setAddr(res);
		return (true);
	}
	return (false);
}

int		checkElements(std::string str){
	size_t occurence = str.find(':', 0);
	int i = 0;
	while (str[i] != ';'){
		if (!isdigit(str[i]) && str[i] != ';' && str[i] != ':' && str[i] != '.')
			return 0;
		i++;
	}
	while (str[++i]){
		if (str[i] != ' ' && str[i] != '\r')
			return 0;
	}

	if (occurence != std::string::npos){
		return (2);
	}
	if (occurence == std::string::npos){
		return (1);
	}
	return (0);
}


bool	serverListenConfig(std::string const & str, Server & current) {
	std::locale	loc;
	int			i = 0;
	int			portTest = 0;
	int			addrTest = 0;
	int			checkreturn = 0;

	if ( ( str.compare(0, 7, "listen ") == 0 || str.compare(0, 7, "listen\t") == 0 ))
	{
		i += 7;
		while ( str[i] == ' ' || str[i] == '\t')
			i++;
		checkreturn = checkElements(str.c_str() + i);
		std::cout << "test0          " << checkreturn << "\n";
		if (checkreturn == 2){

			if ((addrTest = isaddr(str.c_str() + i, current)) == 0) {
				std::cout << "test1\n";
				return (false);
			}
			else if ((portTest = isPort(str.c_str() + str.find(":", 0) + 1, current)) == 0) {
					std::cout << "test2\n";
					return (false);
			}
			else if ((addrTest == true && portTest == true))
				return (true);
		}
		if (checkreturn == 1) {
			if ((addrTest = isaddr(str.c_str() + i, current)) == 0 && (portTest = isPort(str.c_str() + i, current)) == 0)
				return (false);
		}
	}
	return (false);
}
