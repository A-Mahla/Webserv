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
	int i = 0;

	std::cout << "------->TEST " << occurs << "\n";
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


	for (int i = 0; occurs[i] != ';' && occurs[i] != ':' && occurs[i]; i++)
	{
		std::cout << "nouveau test >>>>>>>>>>> " << test << " && " << res << " dot_ct " << dot_ct << " [" << occurs[i] << "] " << "\n";
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
	std::cout << "nouveau test >>>>>>>>>>> " << res << "\n";
	if (dot_ct == 3){
		current.setAddr(res);
		return (true);
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
		i += 7;
		std::cout << str[i] << "\n";
		while ( str[i] == ' ' || str[i] == '\t')
			i++;
		if ( str.compare(i, 10,"localhost:") == 0 || str.compare(i, 2, "*:") == 0 )
		{
			current.setAddr(INADDR_ANY);
			while (str[i] != ';' && str[i] != ':')
				i++;
		}
		addrTest = isaddr( (str.c_str() + i), current );
		portTest = isPort( (str.c_str() + i), current );
		if (!addrTest && !portTest)
			return (false);
		if (!addr && )

		return (true);
	}
	return (false);
}
