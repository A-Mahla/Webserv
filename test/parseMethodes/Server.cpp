#include "Server.h"
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

Server::Server( void ) : _servSock(0), _port(8080), _addr(INADDR_ANY)
{
	std::cout << "in construct : " << INADDR_ANY << std::endl;
}

Server::Server( const int port ) : _servSock(0), _port(port), _addr(INADDR_ANY)
{

}

Server::Server( const Server & rhs )
{
	*this = rhs;
}

Server::~Server( void )
{
}

Server &	Server::operator=( const Server & rhs )
{
	if ( this != &rhs )
	{
		this->_servSock = rhs._servSock;
		this->_port = rhs._port;
		this->_addr = rhs._addr;
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

void			Server::setAddr( int addr ){
	this->_addr = addr;
}

void			Server::setPort( int port ){
	this->_port = port;
}

void			Server::setSock( int sock ){
	this->_servSock = sock;
}

void	Server::print(){
	std::cout << "socket_fd : " << this->_servSock << std::endl;
	std::cout << "port: " << this->_port << std::endl;
	std::cout << "addr : " << this->_addr << std::endl;
}
