/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:51:31 by amahla            #+#    #+#             */
/*   Updated: 2022/11/02 13:18:16 by slahlou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include <iostream>
#include "Response.hpp"
#include "webserv.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>


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
#include <stdlib.h>

#define NB_ELEMENTS 17
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
		if (it->first == path || (it->first[0] == '.' && it->first.substr(1, it->first.size()) == path))
		{
			this->_server = &it->second;
			break ;
		}
		if (!it->second.getLocation().empty())
			get_good_Root(path, &it->second);
	}
}

std::string	Response::readFile(std::string path, Server &serv)
{
	std::ifstream 	ifs;
	std::string		file_content;
	std::string		filename;
	int				length;
	char			*buff;

	get_good_Root(path, &serv);
	if (!this->_server)
		this->_server = &serv;

	filename = this->_server->get_root()+ path.erase(0, 1);

	ifs.open(filename.c_str(), std::ifstream::in);

	if ( !ifs.is_open() )
	{
		this->_status = 404;
		return ("");
	}
	ifs.seekg (0, ifs.end);
	length = ifs.tellg();
	ifs.seekg (0, ifs.beg);

	buff = new char[length + 1];
	ifs.read(buff, length);
	if (ifs.fail())
		this->_status = 403;
	ifs.close();
	buff[length] = '\0';
	file_content = buff;
	delete [] buff;

	return (file_content);
}

void	Response::GET_response(Server & serv, Request & req)
{
	std::string content_str;

	if (*req.getPath().rbegin() != '/')
	{
		content_str = readFile(req.getPath(), serv);
	}
	else if (*req.getPath().rbegin() == '/')
	{
		//autoindex();
		//juste for see if it works
		content_str = "autoindex";
	}

	std::stringstream ss;
	ss << content_str.size();

	if (ss.str().empty())
		return ;

    _response += "HTTP/1.1 200 OK\n";
    _response += "Content-Type: text/html\r\n";
    _response += "Content-Length: ";
    _response += ss.str();
    _response += "\n\n";
    _response += content_str;
    _response += "\r\n\r\n";
}

Response::Response(Server & serv, Request & req) : _server(NULL), _status(0)
{
	if ( DEBUG )
		std::cout << "Response request Constructor" << std::endl;

	_response.clear();

	if (req.getMethode() == GET)
		GET_response(serv, req);
	else if ( req.getMethode() == POST)
	{
		char **test = buildCGIenv(req, serv);
		std::cout << "\n\n--------------ENV------------------\n\n";
		for (int i = 0; *(test + i); i++)
			std::cout << *(test + i) << std::endl;
		std::cout << "\n\n-----------------------------------\n\n";
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
		_response += "9";
		_response += "\n\n";
		_response += "error " + ss.str();
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

/*------------BUILDING CGI ENVIRONNEMENT--------------------*/

char**	Response::buildCGIenv(Request & req, Server & serv)
{
	int i = 0;
	size_t pos = 0;
	std::string	starter = "CONTENT_LENGTH= CONTENT_TYPE= GATEWAY_INTERFACE= PATH_INFO= PATH_TRANSLATED= QUERY_STRING= REMOTE_ADDR= REMOTE_HOST= REMOTE_IDENT= REMOTE_USER= REQUEST_METHOD= SCRIPT_NAME= SERVER_NAME= SERVER_PORT= SERVER_PROTOCOL= SERVER_SOFTWARE= HTTP_COOKIE= ";
	std::string	var[NB_ELEMENTS];

	_initVar(var, req, serv);
	while ((pos = starter.find(" ", pos)) != std::string::npos){
		starter.insert(pos, var[i]);
		pos += var[i].size() + 1;
		i++;
	}
	// std::cout << "\n\n************PRINTING TEST*************\n\n";
	// std::cout << starter << std::endl;
	// //. . . the function take information from server and request to insert into
	// //       starter string.
	// // when everything is set, we split and return the char**

	return (ft_split(starter.c_str(), ' '));
}

void	Response::_initVar(std::string *var, Request const & req, Server const & serv){
	var[0] = "content_lenght"; //req.content_lenght;
	var[1] = "content_type"; //req.content_type;
	var[2] = "CGI/1.1";
	var[3] = req.getPath();
	var[4] = serv.get_root() + req.getPath().substr(1, (req.getPath().size() - 1));
	var[5] = "queryString";//req.querySting;
	var[6] = "originString";//req.origin;
	var[7] = "NULL";
	var[8] =  "NULL"; // client_login;
	var[9] = "NULL"; //user_login;
	if (req.getMethode() == GET)
		var[10] = "GET";
	else if (req.getMethode() == POST)
		var[10] = "POST";
	else if (req.getMethode() == DELETE)
		var[10] = "DELETE";
	var[11]= "test.cgi";//req.scriptName;
	var[12] = req.getAddr();
	std::cout << "TESSSSSSSSSSSSSSSSSSSSSSSSSST " << serv.getPortStr() << std::endl;
	var[13] = serv.getPortStr();
	var[14] = "HTTP/1.1";
	var[15] = "SAMserver/1.0";
	var[16] = "COOKIE";//req.cookieString;
}

/*---------------------- split --------------------*/

int	ft_o(char c1, char c2)
{
	if (c1 == c2)
		return (1);
	else
		return (0);
}

int	ft_count_wrd(char const *s, char c)
{
	int	wrdcount;
	int	i;

	i = 1;
	wrdcount = 0;
	if (!s[0])
		return (wrdcount);
	while (1)
	{
		if ((ft_o(s[i], c) && !ft_o(s[i - 1], c))
			|| (!s[i] && !ft_o(s[i - 1], c)))
			wrdcount++;
		if (!s[i])
			break ;
		i++;
	}
	return (wrdcount);
}

char	*ft_get_wrd(char const *s, int e)
{
	char	*str;
	int		i;

	i = -1;
	str = reinterpret_cast<char *>(malloc(sizeof(char) * (e + 1)));
	if (!str)
		return (NULL);
	while (++i < e)
		str[i] = s[i];
	str[i] = 0;
	return (str);
}

char	*ft_find_wrd(char const *s, char set, int wordneeded)
{
	int	i;
	int	e;

	i = 0;
	e = 0;
	while (i <= wordneeded)
	{
		while (ft_o(*s, set))
			s++;
		while (!ft_o(s[e], set) && s[e])
			e++;
		if (i == wordneeded)
			return (ft_get_wrd(s, e));
		s += e;
		e = 0;
		i++;
	}
	return (NULL);
}

char**	Response::ft_split(char const *s, char c)
{
	char	**strs;
	int		nbwrd;
	int		i;

	nbwrd = ft_count_wrd(s, c);
	strs = reinterpret_cast<char **>(malloc(sizeof(char *) * (nbwrd + 1)));
	if (!strs)
		return (NULL);
	i = -1;
	while (++i < nbwrd)
		*(strs + i) = ft_find_wrd(s, c, i);
	*(strs + i) = NULL;
	return (strs);
}

