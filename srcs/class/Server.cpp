/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:51:31 by amahla            #+#    #+#             */
/*   Updated: 2022/10/24 18:06:21 by slahlou          ###   ########.fr       */
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
}

Server::Server( const int port ) : _servSock(0), _port(port)
{
	if ( DEBUG )
		std::cout << "Server Default Constructor" << std::endl;
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

Server &	Server::operator=( const Server & rhs )
{
	if ( this != &rhs )
	{
		this->_servSock = rhs._servSock;
		this->_port = rhs._port;
	}
	return ( *this );
}

int			& Server::getSock( void )
{
	return ( this->_servSock );
}

const int	& Server::getSock( void ) const
{
	return ( this->_servSock );
}

const int	& Server::getPort( void ) const
{
	return ( this->_port );
}
