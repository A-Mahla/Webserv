/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serv_process.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 12:45:35 by amahla            #+#    #+#             */
/*   Updated: 2022/10/21 22:25:40 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include <unistd.h>
# include <cstring>
# include <netinet/in.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/select.h>
# include <errno.h>
# include <vector>
# include <algorithm>
# include "WebServException.hpp"
# include <cstdlib>
# include <signal.h>
# include <sstream>

void	signal_handler(int sig)
{
	std::stringstream stringBuf;

	stringBuf << "\r" << "==  Webserv was ended by a signal ( sig code : " << sig << " ) ==";
	throw WebServException( stringBuf.str().c_str() );
}

void	setFds( int & maxSock, fd_set* readFds, std::vector<int> & clientSocks, int servSock )
{
	FD_ZERO(readFds);
	FD_SET( servSock, readFds );
	maxSock = servSock;

	for (std::size_t i = 0; i < clientSocks.size(); i++ )
	{
		maxSock = (clientSocks[i] > maxSock) ? clientSocks[i] : maxSock;
		FD_SET(clientSocks[i], readFds);
	}
}

void	waitRequest( fd_set* readFds, std::vector<int> & clientSocks, int servSock )
{
	int				status = 0;
	int				maxSock;
	struct timeval	timeout;

	while (!status)
	{
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		setFds( maxSock, readFds, clientSocks, servSock );

		if (( status = select( maxSock + 1, readFds, NULL, NULL, &timeout )) < 0 )
			throw WebServException( "serv_process.cpp", "waitRequest", "select() failed" );
	}
}

bool	newConnection( int & servSock, std::vector<int> & clientSocks, fd_set* readFds )
{
	struct sockaddr_in	temp;
	int					addrlen = sizeof(temp);
	int					newConnection;

	memset( (char *)&temp, 0, sizeof(temp) );

	if ( FD_ISSET( servSock, readFds ) )
	{
		if ( (( newConnection = accept( servSock, reinterpret_cast< struct sockaddr *>(&temp),
							reinterpret_cast< socklen_t * >(&addrlen) ) ) < 0) )
		{
			if ( errno != EWOULDBLOCK )
				throw WebServException( "serv_process.cpp", "newConnection", "accept() failed" );
		}

		if (newConnection > 0)			
		{
			std::cout << "Connection accepted" << std::endl;
			clientSocks.push_back(newConnection);
			return ( true );
		}
	}

	return ( false );
}

void		ioData( std::vector<int> & clientSocks, fd_set* readFds )
{
	char		buffer_read[1024];
	const char	*buffer_write = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	int			rd;

	for (std::size_t i = 0; i < clientSocks.size(); i++ )
	{
		if ( FD_ISSET( clientSocks[i], readFds ) )
		{
			std::cout << "waiting for recv" << std::endl;
			if ( (rd = recv( clientSocks[i], buffer_read, 1023, 0 )) < 0 )
				throw WebServException( "serv_process.cpp", "ioData", "recv() failed" );
			else if ( rd == 0)
			{
				FD_CLR( clientSocks[i], readFds);
				close( clientSocks[i] );
				clientSocks.erase( clientSocks.begin() + i );
				std::cout << "Connexion client is closed" << std::endl;
			}
			else
			{
				std::cout << "recv successed" << std::endl;
				buffer_read[rd] = '\0';
				std::cout << "Server side receive from client : " << buffer_read << std::endl;
				if ( send( clientSocks[i], buffer_write, strlen(buffer_write), 0 ) < 0 )
					throw WebServException( "serv_process.cpp", "ioData", "send() failed" );
			}
		}
	}
}	

void	appServ( int & servSock )
{
	std::vector<int>	clientSocks;
	fd_set	readFds;

	signal(SIGINT, &signal_handler);

	std::cout << std::endl << "-------------- Server is on ---------------" << std::endl << std::endl;
	try
	{
		while (1)
		{
			waitRequest( &readFds, clientSocks, servSock );
			if ( !newConnection( servSock, clientSocks, &readFds ) )
				ioData( clientSocks, &readFds );
		}
	}
	catch ( std::exception & e )
	{
		std::string	error = e.what();
		FD_CLR( servSock, &readFds );
		close( servSock );
		for (std::size_t i = 0; i < clientSocks.size(); i++ )
		{
			FD_CLR( clientSocks[i], &readFds );
			close( clientSocks[i] );
		}
		throw ( WebServException( e.what() ) ); 
	}
}
