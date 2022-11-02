/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meudier <meudier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:51:31 by amahla            #+#    #+#             */
/*   Updated: 2022/11/01 18:17:12 by meudier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include <iostream>
#include "Client.hpp"

Client::Client( void ) : _clientSock(0), _server(NULL)
{
	if ( DEBUG )
		std::cout << "Client Default Constructor" << std::endl;
}

Client::Client( const int sock ) : _clientSock( sock ), _server(NULL)
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

void	Client::_chooseServer(void)
{
	_server = _serverList[0];
	for (std::vector<Server *>::iterator it = _serverList.begin(); it != _serverList.end(); it++)
	{
		for (std::vector<std::string>::iterator it2 = (*it)->getServerName().begin(); it2 != (*it)->getServerName().end(); it2++)
		{
			if (*it2 == _request.getAddr())
			{
				_server = *it;
				return ;
			}
		}
	}
}

void	Client::setRequest(char * str)
{
	_request.getStringRequest() = str;
	_request.parseRequest();
	if (!_server)
		_chooseServer();	
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

Response		& Client::getResponse( Server & serv, Request & req)
{
	this->_response =  Response( serv, req );
	return ( this->_response );
}

