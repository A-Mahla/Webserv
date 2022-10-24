#include <string>
#include <iostream>
#include <cstdlib>
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

	for (int i = 0 ; occurs[i] != ';' ; i++){
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
	int	dot_count = 0;
	for (int i = 0 ;  occurs[i] == '.' || occurs[i] == ';' ||  occurs[i] == ':'
		|| occurs[i] < '0' || occurs[i] > '9' ; i++)
	{
		test = atoi((occurs.c_str() + i));
		if ( ( test > 255 || test < 0 ) )
			return (false);
		while ( occurs[i] >= '0' && occurs[i] <= '9' )
			i++;
		if (occurs[i] == '.'){
			++dot_count;
			++i;
		}
		else if ((occurs[i] == ';' ||  occurs[i] == ':') && dot_count == 3){
			current.setAddr(res);
			return (true);
		}
		else if ( dot_count == 3 )
			return (false);
		res |= test;
		res <<= 8;

	}
	return (false);

}

bool	serverListenConfig(std::string const & str, Server & current) {
	std::locale	loc;
	int			i = 0;
	int			portTest = 0;
	int			addrTest = 0;

	if ( ( str.compare(0, 7, "listen ") == 0 || str.compare(0, 7, "listen\t") == 0 )  && *(str.end() - 1) == ';')
	{
		std::cout << "ICI LE TEST\n";
		i += 8;
		while ( str[i] == ' ' || str[i] == '\t')
			i++;
		if ( str.compare(i, 10,"localhost:") == 0 || str.compare(i, 2, "*:") == 0 )
		{
			current.setAddr(INADDR_ANY);
			while (str[i] != ';' && str[i] != ':')
				i++;
		}
		if ((portTest = isPort( (str.c_str() + i), current )) == 0 && (addrTest = isaddr( (str.c_str() + i), current ) == 0))
			return (false);
		return (true);
	}
	return (false);
}
