#include "Server.h"
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#define ADDR_MASK1 255


Server::Server( void ) : _servSock(0), _port(8080), _addr(INADDR_ANY), _autoindex(false) {
	//std::cout << "in construct : " << INADDR_ANY << std::endl;
}

Server::Server( const int port ) : _servSock(0), _port(port), _addr(INADDR_ANY), _autoindex(false) {

}

Server::Server( const Server & rhs ) {
	*this = rhs;
}

Server::~Server( void ) {
}

Server &	Server::operator=( const Server & rhs ) {
	if ( this != &rhs )
	{
		this->_servSock = rhs.getSock();
		this->_port = rhs.getPort();
		this->_addr = rhs.getAddr();
		this->_autoindex = rhs.getAutoindex();
	}
	return ( *this );
}

int			& Server::getSock( void ) {
	return ( this->_servSock );
}

const int	& Server::getSock( void ) const {
	return ( this->_servSock );
}
const int	&	Server::getAddr( void ) const {
	return (this->getAddr());
}
const int	& Server::getPort( void ) const {
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

void			Server::setAutoindex(bool onOff){
	this->_autoindex = onOff;
}

bool			Server::getAutoindex(void) const {
	return (_autoindex);
}

void	Server::print(){
	std::cout << "socket_fd : " << this->_servSock << std::endl;
	std::cout << "port: " << this->_port << std::endl;
	{
		std::string autoindex = (this->_autoindex == true) ? "true":"false";
		std::cout << "autoindex: " << autoindex << std::endl;
	}
	int test = ((this->_addr >> 24) & ADDR_MASK1);
	std::cout << "addr: " << test << ".";
	test = ((this->_addr >> 16) & ADDR_MASK1);
	std::cout << test << ".";
	test = ((this->_addr >> 8) & ADDR_MASK1);
	std::cout << test << ".";
	test = ((this->_addr) & ADDR_MASK1);
	std::cout << test << "\n";
}
