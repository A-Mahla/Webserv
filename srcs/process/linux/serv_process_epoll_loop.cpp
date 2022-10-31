/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serv_process_epoll_loop.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meudier <meudier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 12:45:35 by amahla            #+#    #+#             */
/*   Updated: 2022/10/31 16:19:17 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include <cstring>
# include <netinet/in.h>
# include <errno.h>
# include <signal.h>
# include "webserv.h"
# include "epoll.h"
# include <unistd.h>


void	waitRequest( t_epoll & epollVar )
{
	int	timeout = 400;

	if (( epollVar.maxNbFd = epoll_wait( epollVar.epollFd, epollVar.events, MAXFD, timeout) ) < 0 )
		throw WebServException( "serv_process_epoll_process.cpp", "waitRequest", "epoll_wait() failed" );
}

void	readData( std::vector<Client> & clients, itClient it, t_epoll & epollVar, int i )
{
	char	buffer_read[1024];
	int		rd;

	if ( (rd = recv( epollVar.events[i].data.fd , buffer_read, 1023, 0 )) <= 0 )
	{
		if ( rd < 0 )
			std::cout << RED << "Connexion client lost" << SET << std::endl;
		else
			std::cout << RED << "Connexion client is closed" << SET << std::endl;
		clients.erase( it );
		epoll_ctl(epollVar.epollFd, EPOLL_CTL_DEL, epollVar.events[i].data.fd, NULL);
		close( epollVar.events[i].data.fd );
	}
	else
	{
		buffer_read[rd] = '\0';
		// (*it).getRequest().getStringRequest() += buffer_read; For concatenate recv
		(*it).getRequest().getStringRequest() = buffer_read;

		epollVar.new_event.events = EPOLLOUT;
		epollVar.new_event.data.fd = epollVar.events[i].data.fd;
		epoll_ctl( epollVar.epollFd, EPOLL_CTL_MOD, epollVar.events[i].data.fd, &epollVar.new_event);
		std:: cout << GREEN << buffer_read << SET << std::endl;
	}
}

void	sendData( std::vector<Client> & clients, itClient it, t_epoll & epollVar, int i )
{
		const char	*buffer_write = it->getResponse(*(it->getServerList()[0]), it->getRequest()).getStringResponse().c_str();
		
		if ( send( epollVar.events[i].data.fd, buffer_write, strlen(buffer_write), 0 ) < 0 )
		{
			std::cout << RED << "Connexion client lost" << SET << std::endl;
			clients.erase( it );
			epoll_ctl(epollVar.epollFd, EPOLL_CTL_DEL, epollVar.events[i].data.fd, NULL);
			close( epollVar.events[i].data.fd );
		}

		epollVar.new_event.events = EPOLLIN;
		epollVar.new_event.data.fd = epollVar.events[i].data.fd;
		epoll_ctl(epollVar.epollFd, EPOLL_CTL_MOD, epollVar.events[i].data.fd, &epollVar.new_event);
}

bool	ioData( std::vector<Client> & clients, t_epoll & epollVar, int i)
{
	itClient	it = find( clients, epollVar.events[i].data.fd );

	if ( epollVar.events[i].events & EPOLLIN )
	{
		readData( clients, it, epollVar, i );
		return ( true );
	}
	else if ( epollVar.events[i].events & EPOLLOUT )
	{
		sendData( clients, it, epollVar, i );
		return ( true );
	}
	return ( false );
}	

void	addConnection( std::vector<Client> & clients, std::vector< Server > & servers, t_epoll & epollVar, int fdServer )
{
	struct sockaddr_in	temp;
	int					addrlen = sizeof(temp);
	int					newConnection;

	memset( (char *)&temp, 0, sizeof(temp) );

	if ( ( newConnection = accept( fdServer, reinterpret_cast< struct sockaddr *>(&temp),
					reinterpret_cast< socklen_t * >(&addrlen) ) ) < 0 )
	{
		if ( errno != EWOULDBLOCK )
			throw WebServException( "serv_process_epoll_loop.cpp", "newConnection", "accept() failed" );
	}
	

	else if ( newConnection >= 0 )
	{
		for ( int i = 3, bit = 0; i >= 0; i-- )
		{
			int addr = ( temp.sin_addr.s_addr >> bit) & 255;
			std::cout << addr;
			if ( i )
				std::cout << ".";
			bit += 8;
		}
		std::cout << std::endl;
		std::cout << YELLOW << "Connection accepted" << SET << std::endl;
		nonBlockSock( newConnection );

		epollVar.new_event.data.fd = newConnection;
		epollVar.new_event.events = EPOLLIN;
		epoll_ctl( epollVar.epollFd, EPOLL_CTL_ADD, newConnection, &(epollVar.new_event ));
		
		clients.push_back( Client( newConnection ) );
		whichAddrServer( servers, temp, clients.back() );
	}
}

bool	newConnection( std::vector<Client> & clients, std::vector<Server> & servers, t_epoll & epollVar, int i )
{
	int	fdServer;

	if ( epollVar.events[i].events & EPOLLIN &&
		( fdServer = isServer( servers, epollVar.events[i].data.fd ) >= 0 ) )
	
	{
		addConnection( clients, servers, epollVar, fdServer );
		return ( true );
	}
	return ( false );
}

bool	errorEpoll( std::vector<Server> & servers, std::vector<Client> & clients, t_epoll & epollVar, int i )
{
	if ( epollVar.events[i].events & EPOLLERR || epollVar.events[i].events & EPOLLHUP )
	{
		int	fdToClear = epollVar.events[i].data.fd;

		epoll_ctl( epollVar.maxNbFd, EPOLL_CTL_DEL, epollVar.events[i].data.fd, NULL);
		if ( isServer( servers, fdToClear ) >= 0 )
			clients.erase( find( clients, fdToClear ) );
		else
			servers.erase( find( servers, fdToClear ) );
		close( fdToClear );
		return ( true );
	}
	return ( false );
}

void	servProcess( std::vector<Server> & servers, std::vector<Client> & clients, t_epoll & epollVar )
{
	setEpollQueue( epollVar, servers );
	while (1)
	{
		waitRequest( epollVar );
		for ( int i(0); i < epollVar.maxNbFd; i++)
		{
			if ( errorEpoll( servers, clients, epollVar, i ) )
				continue ;
			if ( !newConnection( clients, servers, epollVar, i ) )
				ioData( clients, epollVar, i );
		}
	}
}
