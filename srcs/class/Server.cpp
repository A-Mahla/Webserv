/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:51:31 by amahla            #+#    #+#             */
/*   Updated: 2022/10/25 21:07:59 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netinet/in.h>
#include "defines.h"
#include <iostream>
#include "Server.hpp"

Server::Server( void ) : _servSock(0), _port(8080)
{
	if ( DEBUG )
		std::cout << "Server Default Constructor" << std::endl;
	this->_is_set["listen"] = false;
	this->_is_set["server_name"] = false;
	this->_is_set["location"] = false;
	this->_is_set["autoindex"] = false;
	this->_is_set["clientbody"] = false;
	this->_is_set["error_page"] = false;
	this->_is_set["index"] = false;
	this->_is_set["root"] = false;
}

Server::Server( const Server & rhs )
{
	if ( DEBUG )
		std::cout << "Server copy Constructor" << std::endl;
	*this = rhs;
}

Server::~Server( void )
{
	if ( DEBUG )
		std::cout << "Server Default Destructor" << std::endl;
}

Server &			Server::operator=( const Server & rhs )
{
	if ( this != &rhs )
	{
		this->_servSock = rhs._servSock;
		this->_port = rhs._port;
		this->_serverName = rhs._serverName;
		this->_is_set = rhs._is_set;
	}
	return ( *this );
}

std::map< std::string, bool >	& Server::getMap( void )
{
	return ( this->_is_set );
}

int								& Server::getSock( void )
{
	return ( this->_servSock );
}

const int						& Server::getSock( void ) const
{
	return ( this->_servSock );
}

std::vector< std::string >		& Server::getServerName( void )
{
	return ( this->_serverName );
}

std::map< std::string, Server*>	& Server::getLocation( void )
{
	return ( this->_location );
}

const int						& Server::getPort( void ) const
{
	return ( this->_port );
}
