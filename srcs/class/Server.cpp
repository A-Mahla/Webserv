/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxenceeudier <maxenceeudier@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:51:31 by amahla            #+#    #+#             */
/*   Updated: 2022/10/26 18:32:59 by maxenceeudi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netinet/in.h>
#include "../../headers/defines.h"
#include <iostream>
#include "../../headers/Server.hpp"


/*========================================================*/
/* I ADDED ROOT, CLIENT_BODY AND INDEX WITH DEFAULT VALUE */
/* AND I SETED _IS_SET BY DEFAULT                         */
/*========================================================*/
Server::Server( void ) : _servSock(0), _port(8080), _clientBody(16000), _root("html/")
{
	if ( DEBUG )
		std::cout << "Server Default Constructor" << std::endl;
	_is_set.insert(std::pair<std::string, bool>("clientbody", false));
	_is_set.insert(std::pair<std::string, bool>("error_page", false));
	_is_set.insert(std::pair<std::string, bool>("index", false));
	_is_set.insert(std::pair<std::string, bool>("root", false));
	_index.push_back("index.html");
}


/*========================================================*/
/* I ADDED ROOT, CLIENT_BODY AND INDEX WITH DEFAULT VALUE */
/* AND I SETED _IS_SET BY DEFAULT                         */
/*========================================================*/
Server::Server( const int port ) : _servSock(0), _port(port),  _clientBody(16000), _root("html/")
{
	if ( DEBUG )
		std::cout << "Server Default Constructor" << std::endl;
	_is_set.insert(std::pair<std::string, bool>("clientbody", false));
	_is_set.insert(std::pair<std::string, bool>("error_page", false));
	_is_set.insert(std::pair<std::string, bool>("index", false));
	_is_set.insert(std::pair<std::string, bool>("root", false));
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
		this->_root = rhs._root;
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


/*===================================================*/
/*              MAX GETER AND SETER                  */
/*===================================================*/

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
	_is_set[atribute] = true;
}

vector<string>	&Server::get_index(void)
{
	return (_index);
}

string	Server::get_root(void)
{
	return (_root);
}

void Server::set_root(string root)
{
	_root.clear();
	_root = root;
}