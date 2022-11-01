/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:51:31 by amahla            #+#    #+#             */
/*   Updated: 2022/11/01 14:12:06 by slahlou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "defines.h"
#include "ParseFile.hpp"

Request::Request( void )
{
	if ( DEBUG )
		std::cout << "Request Default Constructor" << std::endl;
}

Request::Request( const Request & rhs )
{
	if ( DEBUG )
		std::cout << "Request copy Constructor" << std::endl;
	*this = rhs;
}

Request::~Request( void )
{
	if ( DEBUG )
		std::cout << "Request Default Destructor" << std::endl;
}

Request &	Request::operator=( const Request & rhs )
{
	if ( this != &rhs )
	{
		this->_request = rhs._request;
		this->_path = rhs._path;
		this->_port = rhs._port;
		this->_addr = rhs._addr;
		this->_method = rhs._method;
	}
	return ( *this );
}

std::string			& Request::getStringRequest( void )
{
	return ( this->_request );
}

const std::string	& Request::getStringRequest( void ) const
{
	return ( this->_request );
}

int			Request::getMethode()
{
	return (_method);
}

std::string	Request::getPath()
{
	return (_path);
}

std::string	Request::getPort()
{
	return (_port);
}

std::string	Request::getAddr()
{
	return (_addr);
}

void	Request::_parseHost( const std::string request )
{
	if ( !request.compare(0, 6, "Host: ") )
	{
			_addr = request.substr(6, request.find(":",6) - 6);
			_port = request.substr((request.find(":", 6) + 1), ( (request.find("\0", 0)) - (request.find(":", 0) + 1) ));
	}
}

void	Request::_parseAccept( const std::string request )
{
	size_t  pos = 0;
	size_t  posTmp = 0;


	if ( !request.compare(0, 8, "Accept: ") )
	{
		_accept.clear();
		posTmp = 8;
		while ((pos = request.find(",", pos)) != std::string::npos)
		{
			_accept.push_back(request.substr(posTmp, pos - posTmp));
			pos++;
			posTmp = pos;
		}
	}
}

bool    Request::_getPath(std::string request)
{
	size_t	pos = 0;
	if ((pos = request.find("HTTP/1.1")) != std::string::npos)
	{
		_path = request.substr( 0, request.find(" ", 0) );
		return (true);
	}
	return (false);
}

void		Request::_parseMethodAndPath(std::string request)
{
	size_t pos = 0;

	if ((pos = request.find("GET")) != std::string::npos)
	{
		if (_getPath(request.substr(4, request.find("\0", 0) - 4)))
			_method = GET;
	}
	else if ((pos = request.find("POST")) != std::string::npos)
	{
		if (_getPath(request.substr(5, request.find("\0", 0) - 5)))
			_method = POST;
	}
	else if ((pos = request.find("DELETE")) != std::string::npos)
	{
		if (_getPath(request.substr(7, request.find("\0", 0) - 7)))
			_method = DELETE;
	}
}


void		Request::parseRequest(void)
{
	std::string			line;
	std::stringstream 	ss;
	size_t				pos;

	ss << this->_request;
	while ((pos = _request.find("\r")) != std::string::npos)
	  	_request.erase(pos, 1);
	while (!ss.eof())
	{
		std::getline(ss, line);
		_parseMethodAndPath(line);
		_parseHost(line);
		_parseAccept(line);
		//std::cout << line << "\n";
	}
	/*std::cout << "*******************************************\n\n";
	std::cout << "\n\n*********PRINTING THE PARSING**********\n\n";

		std::cout << "method = " << _method << "\n";
		std::cout << "_addr = " << _addr << "\n";
		std::cout << "port = " << _port << "\n";
		std::cout << "_path = " << _path << "\n";

		std::cout << "accept options :\n";
		std::cout << "<<\n";
		for (std::vector<std::string>::iterator it = _accept.begin(); it != _accept.end(); it++){
			std::cout << *it << "\n";
		}
		std::cout << ">>" << "\n";
		std::cout << "\n\n***********************************\n\n";*/
}
