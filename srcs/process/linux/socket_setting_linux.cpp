/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_setting_linux.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 12:45:35 by amahla            #+#    #+#             */
/*   Updated: 2022/10/21 22:22:00 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include <cstring>
# include <sys/socket.h>
# include <sys/types.h>
# include <fcntl.h>
# include <unistd.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include "WebServException.hpp"
# include <cstdlib>

int		createSock( void )
{
	int	servSock;
	int	on = 1;

	if (( servSock = socket(AF_INET, SOCK_STREAM, 0 )) < 0 )
	{
		std::cout << "socket() failed" << std::endl;
		exit( EXIT_FAILURE );
	}

	if ( setsockopt( servSock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
		&on, sizeof(int) ) < 0 )
	{
		throw WebServException( "socket_setting.cpp", "createSock", "setsockopt() failed" );
	}

	return ( servSock );
}

void	nonBlockSock( int & servSock )
{
	int flag;

	if ( (flag = fcntl( servSock, F_GETFL )) < 0 )
		throw WebServException( "socket_setting.cpp", "nonBlockSock", "fcntl() F_GETFL failed" );

	if ( fcntl( servSock, F_SETFL, flag | O_NONBLOCK ) < 0 )
		throw WebServException( "socket_setting.cpp", "nonBlockSock", "fcntl() F_SETFL failed" );
}

void	nameSock( int & servSock )
{
	struct sockaddr_in	address;
	const int			port = 8080;
	memset( (char *)&address, 0, sizeof(address) );
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl( INADDR_ANY );
	address.sin_port = htons( port );

	if ( bind( servSock, reinterpret_cast< struct sockaddr * >(&address),
		sizeof(address)) < 0 )
	{
		throw WebServException( "socket_setting.cpp", "nameSock", "bind() failed" );
	}

	if ( listen( servSock, 5) < 0 )
		throw WebServException( "socket_setting.cpp", "nameSock", "listen() failed" );
}

void	setSocket( int & servSock )
{
	try
	{
		servSock = createSock();
		nonBlockSock( servSock );
		nameSock( servSock );
	}
	catch ( std::exception & e )
	{
		close( servSock );
		throw WebServException( e.what() );
	}
}
