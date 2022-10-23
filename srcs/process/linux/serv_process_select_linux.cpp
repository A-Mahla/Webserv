/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serv_process_select_linux.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 12:45:35 by amahla            #+#    #+#             */
/*   Updated: 2022/10/23 20:01:08 by slahlou          ###   ########.fr       */
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
# include "webserv.h"
# include <sys/epoll.h>
# define MAX_EVENTS 100

void	signal_handler(int sig)
{
	std::stringstream stringBuf;

	stringBuf << "\r" << "==  Webserv was ended by a signal ( sig code : " << sig << " ) ==";
	throw WebServException( stringBuf.str().c_str() );
}

// void	setFds( int & maxSock, fd_set* readFds, std::vector<Client> & clients, std::vector<Server> & servers )
// {
// 	FD_ZERO(readFds);

// 	for (std::size_t i = 0; i < servers.size(); i++ )
// 	{
// 		maxSock = (servers[i].getSock() > maxSock) ? servers[i].getSock() : maxSock;
// 		FD_SET( servers[i].getSock(), readFds );
// 	}

// 	for (std::size_t i = 0; i < clients.size(); i++ )
// 	{
// 		maxSock = (clients[i].getSock() > maxSock) ? clients[i].getSock() : maxSock;
// 		FD_SET(clients[i].getSock(), readFds);
// 	}
// }

// void	waitRequest( fd_set* readFds, std::vector<Client> & clients, std::vector<Server> & servers )
// {
// 	int				status = 0;
// 	int				maxSock = 0;
// 	struct timeval	timeout;

// 	while (!status)
// 	{
// 		timeout.tv_sec = 1;
// 		timeout.tv_usec = 0;
// 		setFds( maxSock, readFds, clients, servers );

// 		if (( status = select( maxSock + 1, readFds, NULL, NULL, &timeout )) < 0 )
// 			throw WebServException( "serv_process.cpp", "waitRequest", "select() failed" );
// 	}
// }

// bool	newConnection( std::vector<Server> & servers, std::vector<Client> & clients, fd_set* readFds )
// {
// 	struct sockaddr_in	temp;
// 	int					addrlen = sizeof(temp);
// 	int					newConnection;
// 	bool				is_one_connection = false;

// 	memset( (char *)&temp, 0, sizeof(temp) );

// 	for (std::size_t i = 0; i < servers.size(); i++ )
// 	{
// 		if ( FD_ISSET( servers[i].getSock(), readFds ) )
// 		{
// 			if ( (( newConnection = accept( servers[i].getSock(), reinterpret_cast< struct sockaddr *>(&temp),
// 								reinterpret_cast< socklen_t * >(&addrlen) ) ) < 0) )
// 			{
// 				if ( errno != EWOULDBLOCK )
// 					throw WebServException( "serv_process.cpp", "newConnection", "accept() failed" );
// 			}

// 			if (newConnection > 0)
// 			{
// 				std::cout << YELLOW << "Connection accepted" << SET << std::endl;
// 				nonBlockSock( newConnection );
// 				clients.push_back( Client( newConnection ) );
// 				is_one_connection = true;
// 			}
// 		}
// 	}

// 	return ( is_one_connection ? true : false );
// }

// void	ioData( fd_set* readFds, std::vector<Client> & clients )
// {
// 	char		buffer_read[1024];
// 	const char	*buffer_write;
// 	int		rd;

// 	for (std::size_t i = 0; i < clients.size(); i++ )
// 	{
// 		if ( FD_ISSET( clients[i].getSock(), readFds ) )
// 		{
// 			if ( (rd = recv( clients[i].getSock() , buffer_read, 1023, 0 )) <= 0 )
// 			{
// 				if ( rd < 0 )
// 					std::cout << RED << "Connexion client lost" << SET << std::endl;
// 				else
// 					std::cout << RED << "Connexion client is closed" << SET << std::endl;
// 				FD_CLR( clients[i].getSock(), readFds);
// 				close( clients[i].getSock() );
// 				clients.erase( clients.begin() + i );
// 			}
// 			else
// 			{
// 				buffer_read[rd] = '\0';
// 				// clients[i].getRequest().getStringRequest() += buffer_read; For concatenate recv
// 				clients[i].getRequest().getStringRequest() = buffer_read;

// 				buffer_write = clients[i].getResponse().getStringResponse().c_str();
// 				std::cout << GREEN << "Server side receive from client : " << clients[i].getRequest().getStringRequest() << SET << std::endl;
// 				if ( send( clients[i].getSock(), buffer_write, strlen(buffer_write), 0 ) < 0 )
// 					std::cout << RED << "Connexion client lost" << SET << std::endl;
// 			}
// 		}
// 	}
// }

void	appServ( std::vector<Server> & servers )
{
	std::vector<Client>	clients;
	struct epoll_event ev, events[MAX_EVENTS];
	int epollFd = 0;
	int	event_count = 0;

	signal(SIGINT, &signal_handler);
	try
	{
		epollFd = epoll_create(12); // creating an instance of epoll bound to epollFd;
		for (std::size_t i = 0; i < servers.size(); i++) // adding all my server socket to the epoll instance
		{
			ev.events = EPOLLIN;
			ev.data.fd = servers[i].getSock();
			if (epoll_ctl(epollFd, EPOLL_CTL_ADD, servers[i].getSock(), &ev) < 0){
				//throw ( WebServException( e.what() ) );
				std::cout << "PB INIT EPOLL_CTL AT START L158\n";
				exit(1);
			}
		}
		std::cout << std::endl << "\r" << YELLOW << "-------------- Server is on ---------------" << std::endl << SET << std::endl;
		while (1)
		{
			event_count = epoll_wait(epollFd, events, MAX_EVENTS, 10000);
			for (int i = 0; i < event_count; i++){
				/*******************CHECKING FOR NEW CONNECTION******************/
				for (std::size_t e = 0; e < servers.size(); e++){
					if (events[i].data.fd == servers[e].getSock()){
						int newConnection = -1;
						struct sockaddr_in	temp;
						int					addrlen = sizeof(temp);
						if ( (( newConnection = accept( servers[e].getSock(), reinterpret_cast< struct sockaddr *>(&temp),
									reinterpret_cast< socklen_t * >(&addrlen) ) ) < 0) )
						{
							if ( errno != EWOULDBLOCK )
								throw WebServException( "serv_process.cpp", "newConnection", "accept() failed" );
						}

						if (newConnection > 0)
						{
							std::cout << YELLOW << "Connection accepted" << SET << std::endl;
							nonBlockSock( newConnection );
							clients.push_back( Client( newConnection ) );
							ev.events = EPOLLIN;
							ev.data.fd = newConnection;
							if (epoll_ctl(epollFd, EPOLL_CTL_ADD, newConnection, &ev) < 0){
								throw ( WebServException( "serv", "app", "ctl fail") );
								std::cout << "PB HANDLING NEW CONNECTION AT L189\n";
								exit(1);
							}
						}
					}
				}
				/**********************CHECKING CLIENT REQUEST****************/
				for (std::size_t e = 0; e < clients.size(); e++){
					if (events[i].data.fd == clients[e].getSock())
					{
						int rd = 0;
						char		buffer_read[1024];
						const char	*buffer_write;
						if ( (rd = recv( clients[i].getSock() , buffer_read, 1023, 0 )) <= 0 )
						{
							if ( rd < 0 )
								std::cout << RED << "Connexion client lost" << SET << std::endl;
							else
								std::cout << RED << "Connexion client is closed" << SET << std::endl;
							epoll_ctl(epollFd, EPOLL_CTL_DEL, clients[i].getSock(), NULL);
							close( clients[i].getSock() );
							clients.erase( clients.begin() + i );
						}
						else
						{
							buffer_read[rd] = '\0';
							// clients[i].getRequest().getStringRequest() += buffer_read; For concatenate recv
							clients[i].getRequest().getStringRequest() = buffer_read;

							buffer_write = clients[i].getResponse().getStringResponse().c_str();
							std::cout << GREEN << "Server side receive from client : " << clients[i].getRequest().getStringRequest() << SET << std::endl;
							if ( send( clients[i].getSock(), buffer_write, strlen(buffer_write), 0 ) < 0 )
								std::cout << RED << "Connexion client lost" << SET << std::endl;
						}
					}
				}
			}
			//waitRequest( &readFds, clients, servers );
			//newConnection( servers, clients, &readFds );
			//ioData( &readFds, clients );

		}
	}
	catch ( std::exception & e )
	{
		for (std::size_t i = 0; i < servers.size(); i++ )
		{
			//FD_CLR( servers[i].getSock(), &readFds );
			epoll_ctl(epollFd, EPOLL_CTL_DEL, servers[i].getSock(), NULL);
			close( servers[i].getSock() );
		}
		for (std::size_t i = 0; i < clients.size(); i++ )
		{
			//FD_CLR( clients[i].getSock(), &readFds );
			epoll_ctl(epollFd, EPOLL_CTL_DEL, clients[i].getSock(), NULL);
			close( clients[i].getSock() );
		}
		throw ( WebServException( e.what() ) );
	}
}
