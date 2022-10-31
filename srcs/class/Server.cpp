/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meudier <meudier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:51:31 by amahla            #+#    #+#             */
/*   Updated: 2022/10/31 15:56:09 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netinet/in.h>
#include "defines.h"
#include <iostream>
#include "Server.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>

/*									Value Default:									*/
/*																					*/
/*	server_name	(vector empty)	=> parse : server_name "";					/ none	*/
/*	port		(8080)			=> parse : listen default;					/ none	*/
/*	addr		(INADDR_ANY)	=> parse : listen hostport;					/ none	*/
/*	error_pages	(none)			=> parse :	none									*/
/*	clientBody	(8k/16k)		=> parse :	client_body_buffer_size	8k/16k	/ none	*/
/*	index		(index.html)	=> parse :	index	index.html				/ none	*/
/*	root		(html/)			=> parse :	root	html/					/ none	*/

Server::Server( void ) : _servSock(0), _port(8080), _inetAddr(INADDR_ANY), _clientBody(16000), _autoindex( false ), _root("html/")
{
	if ( DEBUG )
		std::cout << "Server Default Constructor" << std::endl;

	this->_index.push_back("index.html");

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
		this->_serverName = rhs._serverName;
		this->_servSock = rhs._servSock;
		this->_port = rhs._port;
		this->_clientBody = rhs._clientBody;
		this->_root = rhs._root;
		this->_index = rhs._index;
		this->_error_pages = rhs._error_pages;
		this->_is_set = rhs._is_set;
		this->_location = rhs._location;
		this->_autoindex = rhs._autoindex;
		this->_inetAddr = rhs._inetAddr;
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

const int								& Server::getSock( void ) const
{
	return ( this->_servSock );
}

std::vector< std::string >				& Server::getServerName( void )
{
	return ( this->_serverName );
}

std::map< std::string, Server >			& Server::getLocation( void )
{
	return ( this->_location );
}

const int								& Server::getPort( void ) const
{
	return ( this->_port );
}

int										& Server::getPort( void )
{
	return ( this->_port );
}

const in_addr_t							& Server::getInetAddr( void ) const
{
	return (this->_inetAddr);
}

in_addr_t								& Server::getInetAddr( void )
{
	return (this->_inetAddr);
}

std::map< std::string, std::string >	& Server::get_error_pages(void)
{
	return (this->_error_pages);
}

bool									Server::get_is_set( std::string attribute )
{
	return ( this->_is_set[attribute] );
}

void									Server::set_is_set( std::string attribute )
{
	this->_is_set[attribute] = true;
}

int										Server::get_clientBody(void) const
{
	return (this->_clientBody);
}

void									Server::set_clientBody( const int val )
{
	this->_clientBody = val;
}

std::string								Server::get_root(void) const
{
	return ( this->_root );
}

void									Server::set_root( const std::string root )
{
	_root.clear();
	_root = root;
}

std::vector< std::string >				& Server::get_index(void)
{
	return ( this->_index );
}

void									Server::setSock( const int servSock )
{
	this->_servSock = servSock;
}

void									Server::setAddr(in_addr_t addr)
{
	this->_inetAddr = addr;
}

void									Server::setPort( const int port )
{
	this->_port = port;
}

void									Server::setAutoindex( bool onOff )
{
	this->_autoindex = onOff;
}

bool									Server::getAutoindex( void ) const 
{
	return ( this->_autoindex);
}

std::ostream	& operator<<( std::ostream & o, Server rhs )
{
	o << "Server :" << std::endl << std::endl;
	if ( !rhs.getServerName().empty() )
	{
		o << "	server_name: ";
		for ( std::size_t i(0); i < rhs.getServerName().size(); i++ )
			o << "	" << rhs.getServerName()[i] << " ";
		o << std::endl;
	}
	o << "	socket_fd :	" << rhs.getSock() << std::endl;
	o << "	port:		" << rhs.getPort() << std::endl;
	o << "	addr:		";
	for ( int i = 3, bit = 0; i >= 0; i-- )
	{
		int addr = ( rhs.getInetAddr() >> bit) & 255;
		o << addr;
		if ( i )
			o << ".";
		bit += 8;
	}
	o << std::endl << "	clientbody:	" << rhs.get_clientBody() << std::endl;
	o << "	autoindex:	" << (rhs.getAutoindex() ? "true" : "false") << std::endl;
	o << "	root:		" << rhs.get_root() << std::endl;
	if ( !rhs.get_index().empty() )
	{
		o << "	index: " << std::endl;
		for ( std::size_t i(0); i < rhs.get_index().size(); i++ )
			o << "			" << rhs.get_index()[i] << std::endl;
		o << std::endl;
	}
	if ( !rhs.get_error_pages().empty() )
	{
		std::cout << "	map:" << std::endl;
		for ( std::map< std::string, std::string >::iterator it( rhs.get_error_pages().begin() );
				it != rhs.get_error_pages().end(); it++ )
			o << "			" << it->first << " => " << it->second << std::endl;
	}
	if ( !rhs.getLocation().empty() )
	{
		for ( std::map< std::string, Server >::iterator it( rhs.getLocation().begin() );
			it != rhs.getLocation().end(); it++ )
		{
			std::cout << std::endl << "	Location for file";
			o << " \"" << it->first << "\" infos " << it->second;
		}
		o << std::endl;
	}
	return ( o );
}
