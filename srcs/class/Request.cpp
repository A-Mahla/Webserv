/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meudier <meudier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:51:31 by amahla            #+#    #+#             */
/*   Updated: 2022/11/01 13:01:25 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include <iostream>
#include "Request.hpp"
#include "ParseFile.hpp"
#include "sstream"

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

//max

int			Request::getMethode() {return (_method);}
std::string	Request::getPath() {return (_path);}
std::string	Request::getPort() {return (_port);}
std::string	Request::getAddr() {return (_addr);}

bool		Request::_parseMethod(std::string &request)
{
	size_t pos = 0;

	if ((pos = request.find("GET")) != std::string::npos)
	{
		_method = GET;
		request.erase(pos, 3);
	}
	else if ((pos = request.find("POST")) != std::string::npos)
	{
		_method = POST;
		request.erase(pos, 4);
	}
	else if ((pos = request.find("DELETE")) != std::string::npos)
	{
		_method = DELETE;
		request.erase(pos, 6);
	}
	else
	{
		_method = BAD_REQUEST;
		return (false);
	}
	return (true);
}

bool	Request::_parseHost( const std::string str_const )
{
	int				i = 0;
	std::string	str = str_const;

	if ( !str.compare(0, 6, "Host: ") )
	{
		i += 6;
		if ( !checkSyntax( str.c_str() + i ) )
			return ( false );
		str.erase(0, 6);
		if (checkOccurance(str, ":") == 1)
		{
			_addr = str.substr(0, str.find(":", 0));
			_port = str.substr((str.find(":", 0) + 1), ( (str.find(";", 0)) - (str.find(":", 0) + 1) ));
			return (true);
		}
		else
			return (false);
	}
	return (false);
}



void		Request::parseRequest(void)
{
	std::string			line;
	std::stringstream 	ss;
	ss << _request;
	
	while (ss.eof())
		std::getline(ss, line);
	size_t pos = 0;

	while ((pos = _request.find("\r")) != std::string::npos)
	 	_request.erase(pos, 1);

	if (!_parseMethod(_request))
		return ;
	
	if ((pos = _request.find("HTTP/1.1")) != std::string::npos)
	{
		_request.erase(0, 1);
		_path = string(_request.begin(), _request.begin() + pos - 2);
		pos =  _request.find("\n");
	 	_request.erase(0, pos + 1);
	}
	else
		return ;
		
		
	
		
	_request.push_back(';');

	if (!_parseHost(_request))
		return ;
		
	/*===========================================================
	std::cout << RED << "request: " << SET << "method: " << _method <<  "\n";
	std::cout << "port: " << _port << "\n" << "addr: " <<  _addr << "\n" ;
	std::cout << "path: " << _path  <<  std::endl;*/
	
}

