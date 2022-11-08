/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meudier <meudier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:51:31 by amahla            #+#    #+#             */
/*   Updated: 2022/11/08 13:43:55 by meudier          ###   ########.fr       */
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
# include <stdio.h>
# include <string>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <memory>

#define NB_ELEMENTS 17


Response::Response(Server & serv, Request & req, int fd) : _isCGI(0), _status(req.getStatus())
{
	if ( DEBUG )
		std::cout << "Response request Constructor" << std::endl;
	if (_status)
	{
		_getErrorPage(serv);
		return ;
	}

	_fd = fd;
	if (req.getMethode() == GET)
		GET_response(serv, req);
	else if ( req.getMethode() == POST)
		POST_response(serv, req);
	else if ( req.getMethode() == DELETE)
		DELETE_response(serv, req);

	if (_status)
		_getErrorPage(serv);
}

/*------------canonical form--------------------*/
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
		this->_fd = rhs._fd;
		this->_isCGI = rhs._isCGI;
		this->_status = rhs._status;
	}
	return ( *this );
}

Response::Response( void ) : _isCGI(0), _status(0)
{
	if ( DEBUG )
		std::cout << "Response Default Constructor" << std::endl;
	this->_response = "HTTP/1.1 200 OK\n";
	this->_response += "Content-Type: text/plain\n";
	this->_response += "Content-Length: 12";
	this->_response += "\n\nHello world!";
}

/*------------geter--------------------*/
std::string			& Response::getStringResponse( void )
{
	return ( this->_response );
}

const std::string	& Response::getStringResponse( void ) const
{
	return ( this->_response );
}

bool				Response::getIsCGI(void)
{
	return (this->_isCGI);
}

int		&Response::getStatus(void)
{
	return (_status);
}

/*------------public methode--------------------*/

bool		Response::_checkFileToDelete(std::string const & script)
{
	if (script.find("*", 0) == 0)
		return (false);
	else if (script.find(".", 0) == 0 && script.size() == 1)
		return (false);
	else if (script.find("..", 0) != std::string::npos )
		return (false);
	else if (script.find("./", 0) == 0 && script.size() == 2)
		return (false);
	else if (*(script.rbegin()) == '.')
		return (false);
	return (true);
}

void	Response::DELETE_response(Server &serv, Request &req)
{
	std::string script = std::string("/bin/rm -rf ") + serv.get_root() + (req.getPath().substr(1, req.getPath().size() - 1));

	if (_checkFileToDelete(script.substr(12, script.size() - 12)) && _execCGI(script, _buildCGIenv(req, serv)) == 0)
	{
		_response += "HTTP/1.1 204 OK\n";
		_response += "Content-Type: text/html\r\n";
		_response += "Content-Length: ";
		_response += "0";
		_response += "\n\n";
		_response += "\r\n\r\n";
	}
	else{
		_status = 403;
		_getErrorPage(serv);
	}
	_isCGI = false;
}

std::string		Response::_getType(std::string str)
{
	size_t	pos = 0;
	std::string	rslt;

	if ((pos = str.find(".", 0)) != std::string::npos)
		rslt = str.erase(0, pos++);
	return (rslt);
}

void	Response::GET_response(Server & serv, Request & req)
{
	std::string 		content_str;
	std::stringstream 	ss;
	std::string			type;

	if (req.getPath() == "/"  || *req.getPath().rbegin() != '/' )
	{
		if (req.getPath() == "/")
		{
			content_str = _readFile("./html/index.html");
			type = "html";
		}
			
		else
			content_str = _readFile(req.getPath(), serv);
		ss << content_str.size();

		if (ss.str().empty())
			return ;
		
		if (type.empty())
			type = _getType(req.getPath());

		_response += "HTTP/1.1 200 OK\n";
		_response += "Content-Type: text/" + type + "\r\n";
		_response += "Content-Length: ";
		_response += ss.str();
		_response += "\n\n";
		_response += content_str;
		_response += "\r\n\r\n";
	}
	else if (*req.getPath().rbegin() == '/')
	{
		char **env = _buildCGIenv(req, serv);
		std::string	script = "./cgi/autoindex.cgi";
		_execCGI(script, env);
	}
	
}

void	Response::POST_response(Server &serv, Request &req)
{
	char **env = _buildCGIenv(req, serv);
	std::string     script = "./cgi" + _getEnv("PATH_INFO", env);
	_execCGI(script, env);
}


/*------------private methode--------------------*/
std::string	Response::_readFile(std::string path)
{
	std::ifstream 	ifs;
	std::string		file_content;
	std::string		filename;
	int				length;
	char			*buff;

	filename = path;
	
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

std::string	Response::_readFile(std::string path, Server &serv)
{
	std::ifstream 	ifs;
	std::string		file_content;
	std::string		filename;
	int				length;
	char			*buff;

	filename = serv.get_root()+ path.erase(0, 1);
	
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

char    **Response::_getArgv(std::string script)
{
    return (_ft_split(script.c_str(), ' '));
}

void    Response::_clear_argv(char **argv)
{
    free(argv[0]);
    delete [] argv;
}

void		Response::_clear_env(char **env)
{
	int i = 0;

	while (env[i])
		free(env[i++]);
	free(env);
}

std::string Response::_getEnv(std::string key, char **env)
{
	std::string	value;
	std::string	temp;
	int i = 0;
	size_t	pos;

	while (env[i])
	{
		temp = env[i];
		if (temp.find(key, 0) != std::string::npos)
		{
			pos = temp.find("=", 0) + 1;
			value = temp.substr(pos, temp.size() - pos);
			break;
		}
		i++;
	}
	return (value);
}

void	Response::_printErrorPage()
{
	std::stringstream ss1;
	ss1 << _status;
	std::string error_page = "./html/error_page/" + ss1.str() + ".html";
	std::string content_str = _readFile(error_page);
	std::stringstream ss2;
	ss2 << content_str.size();

	std::cout << "HTTP/1.1 200 OK\n";
	std::cout << "Content-Type: text/html\r\n";
	std::cout << "Content-Length: ";
	std::cout << ss2.str();
	std::cout << "\n\n";
	std::cout << content_str;
	std::cout << "\r\n\r\n";
}

void	Response::_getErrorPage(Server &serv)
{
	(void)serv;
	std::stringstream ss1;
	ss1 << _status;
	std::string error;

	/*if (!serv.get_error_pages().empty() && (error = serv.get_error_pages()[ss1.str()] == ))
	{
		
	}*/
	if (serv.get_error_pages().find(ss1.str()) != serv.get_error_pages().end())
	{
		if (serv.get_root()[0] == '/')
			error = ".";
		if ( serv.get_error_pages()[ss1.str()][0] == '/')
			error += serv.get_root().erase(serv.get_root().size() - 1, 1) + serv.get_error_pages()[ss1.str()];
		else
			error += serv.get_root() + serv.get_error_pages()[ss1.str()];
	}
	else
		error =  "./html/error_page/" + ss1.str() + ".html";
	
	std::string error_page = error;
	std::string content_str = _readFile(error_page);
	std::stringstream ss2;
	ss2 << content_str.size();

	_response.clear();
	_response += "HTTP/1.1 200 OK\n";
	_response += "Content-Type: text/html\r\n";
	_response += "Content-Length: ";
	_response += ss2.str();
	_response += "\n\n";
	_response += content_str;
	_response += "\r\n\r\n";
}

int    Response::_execCGI(std::string script, char **env)
{
    int pid;
	int	status;
    char            **argv = _getArgv(script);

    pid = fork();
    if (pid == -1 || !argv)
    {
        std::cout << "error fork()" << std::endl;
        return (-1);
    }

    if (pid == 0)
    {
        dup2(this->_fd, STDOUT_FILENO);
		
        execve(argv[0], argv, env );
		
		_status = 400;
		_printErrorPage();
	
        close(this->_fd);
        _clear_argv(argv);
		_clear_env(env);
		exit(1);
    }
    wait(&status);
    _clear_env(argv);
	_clear_env(env);
	_isCGI = true;
	return (status >> 8);
}

/*------------BUILDING CGI ENVIRONNEMENT--------------------*/

char**	Response::_buildCGIenv(Request const & req, Server const & serv)
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
	return (_ft_split(starter.c_str(), ' '));
}

void	Response::_initVar(std::string *var, Request const & req, Server const & serv){

	var[0] = req.getContentLengthStr();
	var[1] = req.getContentType();
	var[2] = "CGI/1.1";
	var[3] = req.getPath();
	var[4] = serv.get_root() + req.getPath().substr(1, (req.getPath().size() - 1));
	var[5] = req.getQueryString();
	var[6] = req.getOrigin();
	var[7] = req.getAddr();
	var[8] =  "NULL"; // -client_login;
	var[9] = "NULL"; //user_login;
	if (req.getMethode() == GET)
		var[10] = "GET";
	else if (req.getMethode() == POST)
		var[10] = "POST";
	else if (req.getMethode() == DELETE)
		var[10] = "DELETE";
	var[11]= req.getPath();
	var[12] = (static_cast<Server>(serv)).getServerName()[0];
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
	
char**	Response::_ft_split(char const *s, char c)
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

