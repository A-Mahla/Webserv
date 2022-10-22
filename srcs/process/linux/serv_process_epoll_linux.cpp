/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serv_process_epoll_linux.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 12:45:35 by amahla            #+#    #+#             */
/*   Updated: 2022/10/22 20:38:25 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include <unistd.h>
# include <cstring>
# include <netinet/in.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/epoll.h>
# include <errno.h>
# include <vector>
# include <algorithm>
# include "WebServException.hpp"
# include <cstdlib>
# include <signal.h>
# include <sstream>
# include "webserv.h"

void	signal_handler(int sig)
{
	std::stringstream stringBuf;

	stringBuf << "\r" << "==  Webserv was ended by a signal ( sig code : " << sig << " ) ==";
	throw WebServException( stringBuf.str().c_str() );
}

void	setEpollQueue( int & epollFd, std::vector<Server> & servers, struct epoll_event & new_event )
{

	epollFd = epoll_create1(0);
	
	for (std::size_t i = 0; i < servers.size(); i++ )
	{
		new_event.data.fd = servers[i].getSock();
		new_event.events = EPOLLIN;
		epoll_ctl( epollFd, EPOLL_CTL_ADD, servers[i].getSock(), &new_event );
	}

}

void	waitRequest( int & epollFd, int & maxNbFd, struct epoll_event * events )
{
	int	timeout = 400;
	maxNbFd = 0;

	while (!maxNbFd)
	{
		std::cout << "Yooo" << std::endl;
		if (( maxNbFd = epoll_wait( epollFd, events, 1000, timeout) ) < 0 )
			throw WebServException( "serv_process.cpp", "waitRequest", "select() failed" );
	}
}

bool	ioData( std::vector<Client> & clients, struct epoll_event events, int & epollFd )
{
	struct epoll_event	new_event;
	char				buffer_read[1024];
	const char			*buffer_write;
	int					rd;
	Client				client;


	for (std::size_t i = 0; i < clients.size(); i++ )
	{
		if ( clients[i].getSock() == events.data.fd )
			client = clients[i];
	}

	if ( events.events & EPOLLIN )
	{
		if ( (rd = recv( client.getSock() , buffer_read, 1023, 0 )) <= 0 )
		{

			if ( rd < 0 )
				std::cout << RED << "Connexion client lost" << SET << std::endl;
			else
				std::cout << RED << "Connexion client is closed" << SET << std::endl;
			close( events.data.fd );
			for (std::size_t i = 0; i < clients.size(); i++ )
			{
				if ( clients[i].getSock() == events.data.fd )
					clients.erase( clients.begin() + i );
			}	
		}
		else
		{
			buffer_read[rd] = '\0';
			// client.getRequest().getStringRequest() += buffer_read; For concatenate recv
			client.getRequest().getStringRequest() = buffer_read;

			new_event.events = EPOLLOUT;
			new_event.data.fd = events.data.fd;
			epoll_ctl( epollFd, EPOLL_CTL_MOD, events.data.fd, &new_event);

			std::cout << GREEN << "Server side receive from client : " << client.getRequest().getStringRequest() << SET << std::endl;
			return ( true );
		}
	}
	else if ( events.events & EPOLLOUT )
	{
		buffer_write = client.getResponse().getStringResponse().c_str();
		if ( send( client.getSock(), buffer_write, strlen(buffer_write), 0 ) < 0 )
			std::cout << RED << "Connexion client lost" << SET << std::endl;

		new_event.events = EPOLLIN;
		new_event.data.fd = events.data.fd;
		epoll_ctl(epollFd, EPOLL_CTL_MOD, events.data.fd, &new_event);

		return ( true );
	}
	return (false );
}	


void	newConnection( std::vector<Client> & clients, std::vector<Server> & servers, int & maxNbFd, int & epollFd, struct epoll_event * events, struct epoll_event & new_event )
{
	struct sockaddr_in	temp;
	int					addrlen = sizeof(temp);
	int					newConnection;
	bool				is_one_connection = false;

	for ( int i = 0; i < maxNbFd; i++ )
	{
		is_one_connection = false;
		if ( events[i].events & EPOLLERR || events[i].events & EPOLLHUP )
		{
			for (std::size_t j = 0; j < clients.size(); j++ )
			{
				if ( clients[j].getSock() == events[i].data.fd )
					clients.erase( clients.begin() + j );
			}
			close( events[i].data.fd );
			continue ;
		}
		for (std::size_t j = 0; j < servers.size(); j++ )
		{
			if ( servers[j].getSock() == events[i].data.fd )
			{
				memset( (char *)&temp, 0, sizeof(temp) );

				if ( (( newConnection = accept( servers[j].getSock(), reinterpret_cast< struct sockaddr *>(&temp),
								reinterpret_cast< socklen_t * >(&addrlen) ) ) < 0) )
				{
					if ( errno != EWOULDBLOCK )
						throw WebServException( "serv_process.cpp", "newConnection", "accept() failed" );
				}

				if (newConnection > 0)			
				{
					std::cout << YELLOW << "Connection accepted" << SET << std::endl;
					nonBlockSock( newConnection );
					new_event.data.fd = newConnection;
					new_event.events = EPOLLIN;
					epoll_ctl(epollFd, EPOLL_CTL_ADD, newConnection, &new_event);
					clients.push_back( Client( newConnection ) );
				}
				is_one_connection = true;
			}
		}
		if ( !is_one_connection )
			if ( ioData( clients, events[i], epollFd ) )
				break ;
	}
}

void	appServ( std::vector<Server> & servers )
{
	std::vector<Client>	clients;
	struct epoll_event	new_event;
	struct epoll_event	events[10000];
	int					epollFd;
	int					maxNbFd = 0;

	signal(SIGINT, &signal_handler);

	std::cout << std::endl << "\r" << YELLOW << "-------------- Server is on ---------------" << std::endl << SET << std::endl;
	try
	{
		
		setEpollQueue( epollFd, servers, new_event );
		while (1)
		{
			waitRequest( epollFd, maxNbFd, events );
			newConnection( clients, servers, epollFd, maxNbFd, events, new_event );

		}
	}
	catch ( std::exception & e )
	{
		for (std::size_t i = 0; i < servers.size(); i++ )
		{
			epoll_ctl(epollFd, EPOLL_CTL_DEL, servers[i].getSock(), NULL);
			close( servers[i].getSock() );
		}
		for (std::size_t i = 0; i < clients.size(); i++ )
		{
			epoll_ctl(epollFd, EPOLL_CTL_DEL, clients[i].getSock(), NULL);
			close( clients[i].getSock() );
		}
		throw ( WebServException( e.what() ) ); 
	}
}
