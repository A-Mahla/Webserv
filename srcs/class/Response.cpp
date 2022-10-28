/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meudier <meudier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:51:31 by amahla            #+#    #+#             */
/*   Updated: 2022/10/28 16:09:15 by meudier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include <iostream>
#include "Response.hpp"
#include "webserv.h"

#include <sstream>  


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
	char    buff[1024];
	int fd = 0;
	int ret = 1;
	std::string	filename;
	std::string	html;

	std::string request = req.getStringRequest();
	size_t pos = 0;

	if ((pos = request.find("GET")) != std::string::npos)
		request.erase(pos, 3);
	
	if ((pos = request.find("HTTP/1.1")) != std::string::npos)
		request.erase(pos, 8);
		

	if ((pos = request.find("/")) != std::string::npos)
		request.erase(pos, 1);
	
	if ((pos = request.find("\n")) != std::string::npos)
		request.erase(pos, request.size() - pos);

	while ((pos = request.find(" ")) != std::string::npos)
		request.erase(pos, 1);

	while ((pos = request.find("\t")) != std::string::npos)
		request.erase(pos, 1);

	request.erase(request.size() - 1, 1);
	

	if ( DEBUG )
		std::cout << "Response request Constructor" << std::endl;
	//use the request to replace index.html
	
	filename =  serv.get_root() + request;
	if (request == "favicon.ico" || !request.size())
	{
		filename =  serv.get_root() + "index.html";
	}

	
	fd = open(filename.c_str(), O_RDONLY);

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
	
	std::stringstream ss;
	ss << ret;

    _response += "HTTP/1.1 200 OK\n";
    _response += "Content-Type: text/html\r\n";
    _response += "Content-Length: ";
    _response += ss.str();
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

