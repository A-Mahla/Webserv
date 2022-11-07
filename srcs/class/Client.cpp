/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meudier <meudier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:51:31 by amahla            #+#    #+#             */
/*   Updated: 2022/11/07 14:00:16 by meudier          ###   ########.fr       */
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

void	Client::_chooseServer( std::string path )
{
	_server = _serverList[0];
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
	_get_good_Root( path, this->_serverList[0] );
	if ( !this->_server )
		_server = this->_serverList[0];
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
			_chooseServer( this->_request.getPath() );
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
