/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:51:31 by amahla            #+#    #+#             */
/*   Updated: 2022/10/22 17:03:12 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include <iostream>
#include "Client.hpp"

Client::Client( void ) : _clientSock(0)
{
	if ( DEBUG )
		std::cout << "Client Default Constructor" << std::endl;
}

Client::Client( const int socket ) : _clientSock( socket )
{
	if ( DEBUG )
		std::cout << "Client Default Constructor" << std::endl;
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

Response		& Client::getResponse( void )
{
	return ( this->_response );
}

const Response	& Client::getResponse( void ) const
{
	return ( this->_response );
}
