/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meudier <meudier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:51:31 by amahla            #+#    #+#             */
/*   Updated: 2022/11/02 15:52:40 by meudier          ###   ########.fr       */
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

Response::Response( void ) : _server(NULL), _status(0)
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

	//std::cout << "get lenght" << length << std::endl;

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

void	Response::POST_response(Server &serv, Request &req)
{
	(void) serv;
	(void) req;
	_execCGI();
}

Response::Response(Server & serv, Request & req, int fd) : _server(NULL), _status(0)
{
	if ( DEBUG )
		std::cout << "Response request Constructor" << std::endl;

	_response.clear();
	_fd = fd;
	if (req.getMethode() == GET)
		GET_response(serv, req);
	else if ( req.getMethode() == POST)
	{
		/*SACHANT QUE C'EST UNE METHODE POST, ON CHECK SI IL Y A UN CGI ET SI IL EST UTILISABLE*/
		//if (_cgiCheck(req.getPath()))
		//	execCGI(buildEnvVar(serv, req));
		/*SI OUI, LS FONCTION execCGI PREND UN CHAR** POUR EXEC LE CGI,
		 char** RENVOYE PAR LA FONCTION buildEnvVar() */
		 /*VOIR FICHIER test/environnementVarCGI.cpp pour les prototype des fonctions*/
		POST_response(serv, req);
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
		this->_server = rhs._server;
		this->_status = rhs._status;
	}
	return ( *this );
}

std::string			& Response::getStringResponse( void )
{
	return ( this->_response );
}

const std::string	& Response::getStringResponse( void ) const
{  std::cout << "\n"                             << std::endl;
	return ( this->_response );
}



//cgi
char    **Response::_getArgv(std::string script)
{
    char**  argv = new char*[1];
    argv[0] = strdup(script.c_str());
    return (argv);
}

void    Response::_clear_argv(char **argv)
{
    free(argv[0]);
    delete [] argv;
}

void    Response::_execCGI(void)
{
    int pid;
    std::string     script = "./cgi/test.cgi";
    char            **argv = _getArgv(script);
        
    pid = fork();
    if (pid == -1)
    {
        std::cout << "error fork()" << std::endl;
        return ;
    }

    if (pid == 0)
    {
        dup2(this->_fd, STDOUT_FILENO);
        execve(script.c_str(), argv, NULL );
        std::cout << "error execve" << std::endl;
        close(this->_fd);
        _clear_argv(argv);
		exit(1);
    }
    wait(NULL);
    _clear_argv(argv);
	_response = "POST";
}