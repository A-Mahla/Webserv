/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serv_process_epoll_utils.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 12:45:35 by amahla            #+#    #+#             */
/*   Updated: 2022/10/27 19:21:24 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <sstream>
# include "webserv.h"
# include "epoll.h"

void		signal_handler(int sig)
{
	std::stringstream stringBuf;

	stringBuf << "\r" << "==  Webserv was ended by a signal ( sig code : " << sig << " ) ==";
	throw WebServException( stringBuf.str().c_str() );
}

itClient	find( std::vector<Client> & clients, int clientFd )
{
	itClient	it;
	itClient	ite = clients.end();

	for ( it = clients.begin(); it < ite; it++ )
	{
		if ( (*it).getSock() == clientFd )
			break ;
	}
	return ( it );
}

itServer	find( std::vector<Server> & servers, int serverFd )
{
	itServer	it;
	itServer	ite = servers.end();

	for ( it = servers.begin(); it < ite; it++ )
	{
		if ( (*it).getSock() == serverFd )
			break ;
	}
	return ( it );
}

void	setEpollQueue( t_epoll & epollVar, std::vector<Server> & servers )
{

	epollVar.epollFd = epoll_create1(0);
	epollVar.maxNbFd = 0;
	
	for (std::size_t i = 0; i < servers.size(); i++ )
	{
		epollVar.new_event.data.fd = servers[i].getSock();
		epollVar.new_event.events = EPOLLIN;
		epoll_ctl( epollVar.epollFd, EPOLL_CTL_ADD, servers[i].getSock(), &epollVar.new_event );
	}

}

Server	*isServer( std::vector<Server> & servers, int fd )
{
	for ( std::size_t i(0); i < servers.size(); i++ )
	{
		if ( servers[i].getSock() == fd )
			return ( &servers[i] );
	}
	return ( NULL );
}
