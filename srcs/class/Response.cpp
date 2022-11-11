/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:51:31 by amahla            #+#    #+#             */
/*   Updated: 2022/11/11 18:49:10 by slahlou          ###   ########.fr       */
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
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <memory>

#define NB_ELEMENTS 17


Response::Response(Server &serv, Request &req, int fd) : _isCGI(0), _status(req.getStatus()), _serv(serv), _req(req)
{
	if ( DEBUG )
		std::cout << "Response request Constructor" << std::endl;

	_fd = fd;
	if (_status)
	{
		if (_status >= 400)
			_getErrorPage();
		else
		{
			_response += "HTTP/1.1 200\n";
			_response += "Content-Type: text/html\r\n";
			_response += "Content-Length: ";
			_response += "0";
			_response += "\n\n";
			_response += "\r\n\r\n";
		}
		return ;
	}
	if (serv.getRedirect() && _pathMatchRedirect(serv, req))
	{
		REDIR_response(serv.getRedirectStr());
	}
	else if (req.getMethode() == GET)
		GET_response();
	else if ( req.getMethode() == POST)
		POST_response();
	else if ( req.getMethode() == DELETE)
		DELETE_response();
	else
		_status = 400;

	if (_status)
		_getErrorPage();
}

/*------------canonical form--------------------*/
Response::Response( const Response & rhs ):  _serv(_initserv), _req(_initreq)
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
		this->_req = rhs._req;
		this->_serv = rhs._serv;
		this->_buffer = rhs._buffer;
	}
	return ( *this );
}

Response::Response( void ) : _isCGI(0), _status(0), _serv(_initserv), _req(_initreq)
{
	if ( DEBUG )
		std::cout << "Response Default Constructor" << std::endl;
	_serv = Server();
	this->_response = "HTTP/1.1 200\n";
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

std::vector<unsigned char> & Response::getBufferVec( void ){
	return (this->_buffer);
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



void	Response::DELETE_response()
{
	std::string script = std::string("/bin/rm -rf ") + _serv.get_root() + (_req.getPath().substr(1, _req.getPath().size() - 1));

	if (_checkFileToDelete(script.substr(12, script.size() - 12)) && _execCGI(script, _buildCGIenv()) == 0)
	{
		_fillVector(std::string("HTTP/1.1 204\nContent-Type: text/html\r\nContent-Length: 0\n\n\r\n\r\n"));
		_isCGI = false;
	}
	else
	{
		_status = 403;
		_getErrorPage();
	}

}

std::string		Response::_getType(std::string str)
{
	size_t	pos = str.size();
	size_t	len = 0;
	std::string	rslt;

	while (pos && --pos)
	{
		if (str[pos] == '.')
			break;
		len++;
	}
	rslt = str.erase(0, str.size() - len);

	std::cout << rslt << std::endl;

	return (rslt);
}

void	Response::_fillVector(std::string buf){
	for (size_t i = 0; i < buf.size(); i++)
		this->_buffer.insert(this->_buffer.begin() + i, 1, buf[i]);
}

void	Response::fillbuf(unsigned char *buf)
{
	size_t i = 0;
	for (; i < this->_buffer.size(); i++)
	{
		buf[i] = this->_buffer[i];
	}
	buf[i] = '\0';
}

void	Response::GET_response()
{
	std::stringstream 			ss;
	std::string					type;
	std::string					typo;

	if (_req.getPath() == "/"  || *_req.getPath().rbegin() != '/' )
	{
		if (_req.getPath() == "/")
		{
			_readFile("./html/index.html");
			type = "html";
		}
		else
			_readFile(_req.getPath(), _serv);
		if (!this->_status)
		{
			if (type.empty())
				type = _getType(_req.getPath());

			if ( type == "png" || type == "jpeg")
				typo = "image";
			else if (type == "pdf")
				typo = "application";
			else
				typo = "text";

			std::cout << typo + "/" + type << std::endl;

			_fillVector(std::string("HTTP/1.1 200\n") + std::string("Content-Type: ")
				+ std::string(typo) + std::string("/") + std::string(type) + std::string(" \r\n")
				+ std::string("Content-Length: ") + std::string(ss.str()) + std::string("\n\n"));
		}
		else
			return ;
	}
	else if (*_req.getPath().rbegin() == '/' && this->_serv.getAutoindex())
	{
		char **env = _buildCGIenv();
		std::string	script = "./cgi/autoindex.cgi";
		_execCGI(script, env);
	}
	else
		this->_status = 403;

}

void	Response::POST_response()
{
	char **env = _buildCGIenv();
	std::string		path =  _getEnv("PATH_INFO", env);
	std::string		comp = "/html";
	if (path.compare(0, comp.size(), comp) == 0)
		path.erase(0, 5);
	std::string     script = "./cgi" + path;
	_execCGI(script, env);
}

void	Response::REDIR_response(std::string const & redirectStr){
	_fillVector(std::string("HTTP/1.1 302 Found\nContent-Type: text/html\r\nLocation: ") + redirectStr + std::string("\n\r\r\n\r\n"));
}



/*------------private methode--------------------*/
void Response::_readFile(std::string path)
{
	std::ifstream 	ifs;
	std::string		filename;


	filename = path;

	ifs.open(filename.c_str(), std::ifstream::in);
	if (ifs.fail())
		this->_status = 403;
	if ( !ifs.is_open() )
	{
		this->_status = 404;
		return ;
	}
	std::cout << RED << "readfile2\n" << SET << std::endl;
	try
	{
		this->_buffer = std::vector<unsigned char> ((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	}
	catch ( std::exception & e )
	{
		this->_status = 403;
	}
	if (ifs.fail())
		this->_status = 403;
	ifs.close();
}

void Response::_readFile(std::string path, Server &serv)
{
	std::ifstream 	ifs;
	std::string		filename;

	filename = serv.get_root()+ path.erase(0, 1);
	ifs.open(filename.c_str(), std::ios::binary);
	if (ifs.fail())
		this->_status = 403;
	if ( !ifs.is_open() )
	{
		this->_status = 404;
		return ;
	}
	try
	{
		this->_buffer = std::vector<unsigned char> ((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	}
	catch ( std::exception & e )
	{
		this->_status = 403;
	}
	ifs.close();
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
	std::string error;
	std::string	type;
	std::string	typo;

	if ( _serv.get_error_pages().find(ss1.str()) != _serv.get_error_pages().end())
	{
		if (_serv.get_root()[0] == '/')
			error = ".";
		if ( _serv.get_error_pages()[ss1.str()][0] == '/')
			error += _serv.get_root().erase(_serv.get_root().size() - 1, 1) + _serv.get_error_pages()[ss1.str()];
		else
			error += _serv.get_root() + _serv.get_error_pages()[ss1.str()];
	}
	else
	{
		error =  "./html/error_page/" + ss1.str() + ".html";
		type = "html";
	}

	if (type.empty())
		type = _getType(error);


	if ( type == "png" || type == "jpeg")
		typo = "image";
	else if (type == "pdf")
		typo == "application";
	else
		typo = "text";

	std::string error_page = error;
	/*std::vector<unsigned char> content_str = */_readFile(error_page);
	std::stringstream ss2;
	//ss2 << content_str.size();
	ss2 << this->_buffer.size();

	std::cout << "HTTP/1.1 " + ss1.str() + "\n";
	std::cout << "Content-Type: " + typo + "/" + type + "\r\n";
	std::cout << "Content-Length: ";
	std::cout << ss2.str();
	std::cout << "\n\n";
	for (size_t i(0); i < this->_buffer.size(); i++)
		std::cout << this->_buffer[i];
	std::cout << "\r\n\r\n";
}


void	Response::_getErrorPage()
{
	std::stringstream ss1;
	ss1 << _status;
	std::string error;
	std::string	type;
	std::string	typo;

	if ( _serv.get_error_pages().find(ss1.str()) != _serv.get_error_pages().end())
	{
		if (_serv.get_root()[0] == '/')
			error = ".";
		if ( _serv.get_error_pages()[ss1.str()][0] == '/')
			error += _serv.get_root().erase(_serv.get_root().size() - 1, 1) + _serv.get_error_pages()[ss1.str()];
		else
			error += _serv.get_root() + _serv.get_error_pages()[ss1.str()];
	}
	else
	{
		error =  "./html/error_page/" + ss1.str() + ".html";
		type = "html";
	}

	if (type.empty())
		type = _getType(error);


	if ( type == "png" || type == "jpeg")
		typo = "image";
	else if (type == "pdf")
		typo == "application";
	else
		typo = "text";

	std::string error_page = error;
	_readFile(error_page);
	std::stringstream ss2;
	ss2 << this->_buffer.size();
	_fillVector(std::string("HTTP/1.1 200\n") + std::string("Content-Type: ")
				+ std::string(typo) + std::string("/") + std::string(type) + std::string(" \r\n")
				+ std::string("Content-Length: ") + std::string(ss2.str()) + std::string("\n\n"));
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


bool	Response::_compareLocation(std::string servRedir, std::string reqPath)
{
	int i = 0;
	for (; servRedir[i] == '/' || servRedir[i] == '.'; i++){}
	if (reqPath.find(servRedir.c_str() + i, 0) == std::string::npos)
		return (true);
	else
		return (false);
}

bool		Response::_pathMatchRedirect(Server &serv, Request &req){
	return (_compareLocation(serv.getRedirectStr(), req.getPath()));
}


/*------------BUILDING CGI ENVIRONNEMENT--------------------*/

char**	Response::_buildCGIenv()
{
	int i = 0;
	size_t pos = 0;
	std::string	starter = "CONTENT_LENGTH= CONTENT_TYPE= GATEWAY_INTERFACE= PATH_INFO= PATH_TRANSLATED= QUERY_STRING= REMOTE_ADDR= REMOTE_HOST= REMOTE_IDENT= REMOTE_USER= REQUEST_METHOD= SCRIPT_NAME= SERVER_NAME= SERVER_PORT= SERVER_PROTOCOL= SERVER_SOFTWARE= HTTP_COOKIE= ";
	std::string	var[NB_ELEMENTS];

	_initVar(var);
	while ((pos = starter.find(" ", pos)) != std::string::npos){
		starter.insert(pos, var[i]);
		pos += var[i].size() + 1;
		i++;
	}
	return (_ft_split(starter.c_str(), ' '));
}

void	Response::_initVar(std::string *var){

	var[0] = _req.getContentLengthStr();
	var[1] = _req.getContentType();
	var[2] = "CGI/1.1";
	var[3] = _req.getPath();
	var[4] = _serv.get_root() + _req.getPath().substr(1, (_req.getPath().size() - 1));
	var[5] = _req.getQueryString();
	var[6] = _req.getOrigin();
	var[7] = _req.getAddr();
	var[8] =  "NULL"; // -client_login;
	var[9] = "NULL"; //user_login;
	if (_req.getMethode() == GET)
		var[10] = "GET";
	else if (_req.getMethode() == POST)
		var[10] = "POST";
	else if (_req.getMethode() == DELETE)
		var[10] = "DELETE";
	var[11]= _req.getPath();
	var[12] = (static_cast<Server>(_serv)).getServerName()[0];
	var[13] = _serv.getPortStr();
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

