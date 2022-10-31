/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meudier <meudier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:51:31 by amahla            #+#    #+#             */
/*   Updated: 2022/10/31 18:18:10 by meudier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include <iostream>
#include "Response.hpp"
#include "webserv.h"
#include <fstream>
#include <sstream>  


/*=====================*/
#include <unistd.h>
#include <fcntl.h>
#include <memory>
using namespace std;
/*=====================*/

Response::Response( void ) : _server(NULL)
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

Server	*Response::getServer(void)
{
	return (_server);
}

int	&Response::getStatus(void)
{
	return (_status);
}

void	Response::get_good_Root(std::string path, Server *serv)
{
	for (std::map<std::string, Server>::iterator it = serv->getLocation().begin(); it != serv->getLocation().end(); it++)
	{
		if (it->first == path)
		{
			this->_server = serv;
			break ;
		}
		if (!it->second.getLocation().empty())
			get_good_Root(path, &it->second);
	}
}

void	Response::readFile(std::string path, Server &serv)
{
	get_good_Root(path, &serv);
	if (!this->_server)
		this->_server = &serv;
	std::ifstream ifs;
	
	std::string filename = this->_server->get_root()+ path.erase(0, 1);

	ifs.open(filename.c_str(), std::ifstream::in);

	std::cout << "filename: " << filename << std::endl;
	
	if ( !ifs.is_open() )
		_status = 404;
	
	
	
		
	
}

/*void	Response::setResponse(Server & serv, Request & req)
{
	/char	buff[4096];
	int fd = 0;
	int ret = 1;
	std::string	filename;
	std::string	html;

	filename =  serv.get_root() + request;
	if (request == "favicon.ico" || !request.size())
	{
		filename =  serv.get_root() + "index.html";
	}

	
	fd = open(filename.c_str(), O_RDONLY);

	// return the good error html page
	if (fd > 0)
	{
		ret = read(fd, &buff, 4095);
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

	

	
}*/

Response::Response(Server & serv, Request & req) : _server(NULL), _status(0)
{
	(void)serv;
	
	if ( DEBUG )
		std::cout << "Response request Constructor" << std::endl;
	
	if (req.getMethode() == GET)
	{
		readFile(req.getPath(), serv);
		//req.setStatus(404);
		_response += "HTTP/1.1 200 OK\n";
		_response += "Content-Type: text/html\r\n";
		_response += "Content-Length: ";
		_response += "11";
		_response += "\n\n";
		_response += "GET request";
		_response += "\r\n\r\n";
	}
	else if ( req.getMethode() == POST)
	{
		_response += "HTTP/1.1 200 OK\n";
		_response += "Content-Type: text/html\r\n";
		_response += "Content-Length: ";
		_response += "12";
		_response += "\n\n";
		_response += "POST request";
		_response += "\r\n\r\n";
	}
	else if ( req.getMethode() == DELETE)
	{
		_response += "HTTP/1.1 200 OK\n";
		_response += "Content-Type: text/html\r\n";
		_response += "Content-Length: ";
		_response += "14";
		_response += "\n\n";
		_response += "DELETE request";
		_response += "\r\n\r\n";
	}
	else
		_status = 400;

	
	
	if (_status)
	{
		std::stringstream ss;
		ss << _status;
		_response.clear();
		_response += "HTTP/1.1 200 OK\n";
		_response += "Content-Type: text/html\r\n";
		_response += "Content-Length: ";
		_response += "3";
		_response += "\n\n";
		_response += ss.str();
		_response += "\r\n\r\n";
	}
	
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

