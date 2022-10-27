/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:51:31 by amahla            #+#    #+#             */
/*   Updated: 2022/10/27 19:15:49 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include <iostream>
#include "Response.hpp"
#include "webserv.h"


Response::Response( void )
{
	if ( DEBUG )
		std::cout << "Response Default Constructor" << std::endl;

	this->_response = "HTTP/1.1 200 OK\n";
	this->_response += "Content-Type: text/plain\n";
	this->_response += "Content-Length: 12";
	this->_response += "\n\nHello world!";

}

//			Test MAX
# include <string>
#include <unistd.h>
#include <fcntl.h>

Response::Response(Server & serv, Request & req)
{
	(void)req;
//	std::allocator<char> alloc;
	char    buff[1024];
	int fd = 0;
	int ret = 1;
	std::string	filename;
	std::string	html;

	if ( DEBUG )
		std::cout << "Response request Constructor" << std::endl;
	//use the request to replace index.html
	filename =  serv.get_root() + req.getStringRequest();
	if ((fd = open(filename.c_str(), O_RDONLY)) < 0)
		std::cerr << "bad file\n";
	// return the good error html page
	if (fd > 0)
	{
		ret = read(fd, &buff, 1023);
		buff[ret] = '\0';
		html = buff;
		close (fd);
	}
	else
	{
		//we can't use erno
		fd = open("html/404.html", O_RDONLY);
		ret = read(fd, &buff, 1023);
		buff[ret] = '\0';
		html = buff;
		close (fd);
	}

    _response += "HTTP/1.1 200 OK\n";
    _response += "Content-Type: text/html\r\n";
    _response += "Content-Length: ";
    _response += ret;
    _response += "\n\n";
    _response += html;
    _response += "\r\n\r\n";
}

//				test MAX

Response::Response( const Response & rhs )
{
	if ( DEBUG )
		std::cout << "Response copy Constructor" << std::endl;
	*this = rhs;
}

Response::~Response( void )
{
	if ( DEBUG )
		std::cout << "Response Default Destructor" << std::endl;
}

Response &	Response::operator=( const Response & rhs )
{
	if ( this != &rhs )
	{
		this->_response = rhs._response;
	}
	return ( *this );
}

std::string			& Response::getStringResponse( void )
{
	return ( this->_response );
}

const std::string	& Response::getStringResponse( void ) const
{
	return ( this->_response );
}

