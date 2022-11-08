/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meudier <meudier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:51:31 by amahla            #+#    #+#             */
/*   Updated: 2022/11/08 14:31:48 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include <iostream>
#include "Client.hpp"

Client::Client( void ) : _clientSock(0), _readStatus(1), _server(NULL)
{
	if ( DEBUG )
		std::cout << "Client Default Constructor" << std::endl;
}

Client::Client( const int sock ) : _clientSock( sock ), _readStatus(1), _server(NULL)
{
}

Client::Client( const Client & rhs )
{
	if ( DEBUG )
		std::cout << "Client copy Constructor" << std::endl;
	*this = rhs;
}

Client::~Client( void )
{
	if ( DEBUG )
		std::cout << "Client Default Destructor" << std::endl;
}

Client &	Client::operator=( const Client & rhs )
{
	if ( this != &rhs )
	{
		this->_clientSock = rhs._clientSock;
		this->_request = rhs._request;
		this->_response = rhs._response;
		this->_serverList = rhs._serverList;
		this->_server = rhs._server;
		this->_readStatus = rhs._readStatus;
	}
	return ( *this );
}

int				& Client::getSock( void )
{
	return ( this->_clientSock );
}

const int		& Client::getSock( void ) const
{
	return ( this->_clientSock );
}

Request			& Client::getRequest( void )
{
	return ( this->_request );
}

const Request	& Client::getRequest( void ) const
{
	return ( this->_request );
}

void	Client::_get_good_Root(std::string path, Server *serv)
{
	for (std::map<std::string, Server>::iterator it = serv->getLocation().begin(); it != serv->getLocation().end(); it++)
	{
		if (it->first == path || (it->first[0] == '.' && it->first.substr(1, it->first.size()) == path))
		{
			this->_server = &it->second;
			break ;
		}
		if (!it->second.getLocation().empty())
			_get_good_Root(path, &it->second);
	}
}

void	Client::_chooseServer( std::string path, t_epoll & epollVar, int i )
{
	for (std::vector<Server *>::iterator it = _serverList.begin(); it != _serverList.end(); it++)
	{
		for (std::vector<std::string>::iterator it2 = (*it)->getServerName().begin(); it2 != (*it)->getServerName().end(); it2++)
		{
			if (*it2 == _request.getAddr())
			{
				_get_good_Root( path, *it );
				if ( !this->_server )
					_server = *it;
				return ;
			}
		}
	}
	if ( !this->_server )
	{
		epoll_ctl(epollVar.epollFd, EPOLL_CTL_DEL, epollVar.events[i].data.fd, NULL);
		close( epollVar.events[i].data.fd );	
		std::cout << RED << "Invalid hostname from client" << std::endl;
		std::cout << "Client request rejected" << SET << std::endl;
		this->_readStatus = 0;
	}
}

void	Client::setRequest( t_epoll & epollVar, int i )
{
	if ( !this->_request.getIsSetRequest() )
	{
		if ( ( this->_readStatus = this->_request.readData( this->_clientSock,
			4096, 0, epollVar, i ) ) <= 0 || this->_request.getStatus() )
			return ;
		if ( this->_request.getIsSetRequest() )
		{
			this->_request.parseRequest( epollVar, i );
			if ( this->_request.getStatus() )
				return ;
			if ( !this->_server )
				_chooseServer( this->_request.getPath(), epollVar, i );
			if ( this->_readStatus == 0 )
				return ;
			this->_request.checkMethodeAllowed( *(this->_server) );
			if ( this->_request.getStatus() )
				return ;
		}
	}
	else if ( this->_request.getIsSetRequest() )
	{
		if ( ( this->_readStatus = this->_request.readData( this->_clientSock,
			this->_server->get_clientBody(), 1, epollVar, i ) ) <= 0 || this->_request.getStatus() )
			return ;
	}
	if ( this->_request.getIsSetRequest()
		&& !this->_request.getBoundary().empty() )
	{
		if ( !this->_request.getIsSetHeaderFile() )
			this->_request.parseHeaderFile( *(this->_server), epollVar, i );
		if ( this->_request.getStatus() )
			return ;
		if ( this->_request.getIsSetHeaderFile() )
			this->_request.writeFile( *(this->_server), epollVar, i );
	}
}

Response		& Client::getResponse( void )
{
	return ( this->_response );
}

const Response	& Client::getResponse( void ) const
{
	return ( this->_response );
}

Server			* Client::getServer( void )
{
	return ( this->_server );
}

std::vector< Server * >	& Client::getServerList( void )
{
	return ( this->_serverList );
}

Response		& Client::getResponse( Server & serv, Request & req, int fd)
{
	this->_response =  Response( serv, req, fd);
	return ( this->_response );
}

int				Client::getReadStatus( void )
{
	return ( this->_readStatus );
}
