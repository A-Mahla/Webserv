/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxenceeudier <maxenceeudier@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:51:31 by amahla            #+#    #+#             */
/*   Updated: 2022/10/25 17:00:13 by maxenceeudi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netinet/in.h>
#include "../../headers/defines.h"
#include <iostream>
#include "../../headers/Server.hpp"

Server::Server( void ) : _servSock(0), _port(8080), _clientBody(16000)
{
	if ( DEBUG )
		std::cout << "Server Default Constructor" << std::endl;
	_is_set.insert(std::pair<std::string, bool>("clientbody", false));
	_is_set.insert(std::pair<std::string, bool>("error_page", false));
	_is_set.insert(std::pair<std::string, bool>("index", false));
	_index.push_back("index.html");
}

Server::Server( const int port ) : _servSock(0), _port(port),  _clientBody(16000)
{
	if ( DEBUG )
		std::cout << "Server Default Constructor" << std::endl;
	_is_set.insert(std::pair<std::string, bool>("clientbody", false));
	_is_set.insert(std::pair<std::string, bool>("error_page", false));
	_is_set.insert(std::pair<std::string, bool>("index", false));
	_index.push_back("index.html");
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
		this->_is_set = rhs._is_set;
		this->_clientBody = rhs._clientBody;
		this->_error_pages = rhs._error_pages;
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

int	Server::get_clientBody(void)
{
	return (this->_clientBody);
}

map<string, string>	&Server::get_error_pages(void)
{
	return (this->_error_pages);
}

void	Server::set_clientBody(int val)
{
	_clientBody = val;
}

bool	Server::get_is_set(string atribute)
{
	return (_is_set[atribute]);
}

void	Server::set_is_set(string atribute)
{
	_is_set[atribute] = !_is_set[atribute];
}

vector<string>	&Server::get_index(void)
{
	return (_index);
}
