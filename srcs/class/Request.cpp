/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meudier <meudier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:51:31 by amahla            #+#    #+#             */
/*   Updated: 2022/11/07 15:40:17 by meudier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "defines.h"
#include "ParseFile.hpp"
#include <sstream>

Request::Request( void ) : _isSetRequest(false), _isSetHeaderFile(false),
	_isSetContentLength(false), _contentLength(0), _sizeFile(0), _status(0)
{
	if ( DEBUG )
		std::cout << "Request Default Constructor" << std::endl;
}

Request::Request( const Request & rhs ) : _isSetRequest(false),
	_isSetHeaderFile(false), _isSetContentLength(false), _contentLength(0), _sizeFile(0), _status(0)
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
		this->_isSetRequest = rhs._isSetRequest;
		this->_isSetHeaderFile = rhs._isSetHeaderFile;
		this->_isSetContentLength = rhs._isSetContentLength;
		this->_contentLength = rhs._contentLength;
		this->_contentType = rhs._contentType;
		this->_boundary = rhs._boundary;
		this->_path = rhs._path;
		this->_port = rhs._port;
		this->_addr = rhs._addr;
		this->_accept = rhs._accept;
		this->_contentDisposition = rhs._contentDisposition;
		this->_origin = rhs._origin;
		this->_queryString = rhs._queryString;
		this->_sizeFile = rhs._sizeFile;
		this->_lastNewLineFile = rhs._lastNewLineFile;
		this->_status = rhs._status;
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

int			Request::getMethode() const
{
	return (_method);
}

std::string	Request::getPath() const
{
	return (_path);
}

void	Request::setPath(std::string path)
{
	_path = path;
}

std::string	Request::getPort() const
{
	return (_port);
}

std::string	Request::getAddr() const
{
	return (_addr);
}

void	Request::setStatusError( int status, t_epoll & epollVar, int i )
{
	this->_status = status;
	changeEpollEvent( epollVar, i );
}

void	Request::_parseHost( const  std::string request )
{
	if ( !request.compare(0, 6, "Host: ") )
	{
			_addr = request.substr(6, request.find(":",6) - 6);
			_port = request.substr((request.find(":", 6) + 1), ( (request.find("\0", 0)) - (request.find(":", 6) + 1) ));
	}
}

void	Request::_checkUserAgent( const std::string request )
{
	if (!request.compare(0, 12, "User-Agent: "))
	{
		if (request.find("Firefox", 0) == std::string::npos){
			//_method = BAD_REQUEST;
		}
	}
}

void	Request::_parseAccept( std::string request )
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
		_accept.push_back(request.substr(posTmp, request.size() - posTmp));
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
		{
			_method = GET;
		}
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

size_t			Request::getContentLength( void ) const
{
	return ( this->_contentLength );
}

size_t			Request::getSizeFile( void ) const
{
	return ( this->_sizeFile );
}

std::string const & Request::getContentLengthStr( void ) const
{
	return ( this->_contentLenghtStr );
}

std::string		Request::getContentType( void ) const
{
	return ( this->_contentType );
}

std::string		Request::getBoundary( void ) const
{
	return ( this->_boundary );
}

std::string		Request::getOrigin( void ) const
{
	return ( this->_origin );
}

std::string		Request::getQueryString( void ) const
{
	return ( this->_queryString );
}

bool			Request::getIsSetRequest( void ) const
{
	return ( this->_isSetRequest );
}

bool			Request::getIsSetHeaderFile( void ) const
{
	return ( this->_isSetHeaderFile );
}

bool			Request::getIsSetContentLength( void ) const
{
	return ( this->_isSetContentLength );
}

std::map< std::string, std::string >	& Request::getContentDisposition( void )
{
	return ( this->_contentDisposition );
}

void		Request::_parseOrigin( std::string request )
{
	if ( !request.compare( 0, 8, "Origin: ") )
		this->_origin = request.substr( 8, ( request.size() - 8 ) );
}

void		Request::_parseContentLength( std::string request )
{
	if ( !request.compare( 0, 16, "Content-Length: ") )
	{
		this->_isSetContentLength = true;
		this->_contentLength = std::strtoul( request.substr( 16, ( request.find( "\0", 0 ) - 16 ) ).c_str() , NULL, 0 );
		this->_contentLenghtStr = request.substr( 16, ( request.find( "\0", 0 ) - 16 ) );
	}
}

void		Request::_parseContentType( std::string request )
{
	if ( !request.compare( 0, 14, "Content-Type: ") && request.find( "boundary=", 0 ) != std::string::npos )
	{
		this->_contentType = request.substr( 14, ( request.find( ";", 0 ) - 14 ) );
		this->_boundary = request.substr( request.find_last_of( "-" ) + 1,
			request.find( "\0", 0 ) - request.find_last_of( "-" ) + 1 );
	}
	else if ( !request.compare( 0, 14, "Content-Type: ") )
		this->_contentType = request.substr( 14, ( request.find( "\0", 0 ) - 14 ) );
}

void		Request::_parseContentDisposition( std::string request )
{
	if ( !request.compare( 0, 21, "Content-Disposition: ") )
	{
		this->_contentDisposition["content-type"] = request.substr( 21, ( request.find( ";", 0 ) - 21 ) );

		this->_contentDisposition["name"] = request.substr( request.find( "=", 0 ) + 2,
			( (request.find_last_of( ";" ) - 1) - (request.find( "=", 0 ) + 2 )) );

		this->_contentDisposition["filename"] = request.substr( request.find_last_of( "=" ) + 2,
			 ( request.size() - 2 ) - ( request.find_last_of( "=" ) + 2  ) );
	}
}

void		Request::checkMethodeAllowed( Server & server )
{
	if ( ( this->_method == GET && !server.getAllowGet() )
		|| ( this->_method == DELETE && !server.getAllowDelete() )
		|| ( this->_method == POST && !server.getAllowPost() ) )
		this->_status = 405;
}

void		Request::changeEpollEvent( t_epoll & epollVar, int i )
{
	epollVar.new_event.events = EPOLLOUT;
	epollVar.new_event.data.fd = epollVar.events[i].data.fd;
	epoll_ctl( epollVar.epollFd, EPOLL_CTL_MOD, epollVar.events[i].data.fd, &epollVar.new_event);
}

void		Request::writeFile( Server & server, t_epoll & epollVar, int i )
{
	std::stringstream 	ss;
	std::string			line;
	std::string			temp;

	if ( this->_sizeFile != this->_contentLength )
		if ( this->_request.find( "\n", 0 ) == std::string::npos )
			return ;

	temp = this->_request;
	this->_request = this->_lastNewLineFile;
	this->_request += temp;
	this->_lastNewLineFile.clear();

	ss << this->_request;
	while ( std::getline(ss, line) && !ss.eof() )
	{
		
		if ( line.find( this->_boundary, 0 ) != std::string::npos )
			break ;
		if ( line.find( "\r", 0 ) == 0 )
			continue ;
		this->_newFile << line;
		this->_newFile << "\n";
	}
	if ( line.find( this->_boundary, 0 ) != std::string::npos )
	{
		temp = this->_request.substr( this->_request.find( this->_boundary, 0),
			this->_request.size() - 1 );
		this->_request = temp;
		this->_newFile.close();
		this->_contentDisposition.clear();
		this->_isSetHeaderFile = false;
		parseHeaderFile( server, epollVar, i );
	}
	else
	{
		this->_request.clear();
		this->_lastNewLineFile = line;
	}
}

void		Request::parseHeaderFile( Server & server, t_epoll & epollVar, int i )
{
	std::string	line;
	std::stringstream 	ss;
	std::string			temp;
	std::string			nameFile;

	if ( this->_sizeFile == this->_contentLength )
		changeEpollEvent( epollVar, i );

	if ( this->_request.find( "\r\n\r\n", 0 ) == std::string::npos )
		return ;

	temp = this->_request.substr( this->_request.find( "\r\n\r\n", 0 ) + 4,
		this->_request.size()- 1 );
	this->_isSetHeaderFile = true;
	this->_contentType.clear();
	ss << this->_request;
	while (!ss.eof())
	{
		std::getline(ss, line);
		_parseContentType( line );
		_parseContentDisposition( line );
		if ( !this->_contentType.empty() )
			break ;
	}

	if ( this->_isSetHeaderFile )
	{
		if ( server.get_root()[0] == '/' )
			nameFile = ".";
		nameFile += server.get_root();

		for( itMapStringString it = this->_contentDisposition.begin();
				it != this->_contentDisposition.end(); it++ )
		{
			if ( it->first == "filename" )
				nameFile += it->second;
		}
		this->_newFile.clear();
		this->_newFile.open( nameFile.c_str(), std::ofstream::out );
		if ( !this->_newFile.is_open() )
		{
			setStatusError( 403, epollVar, i );
			return ;
		}
		this->_request = temp;
		writeFile( server, epollVar, i );
	}
}

void		Request::parseRequest( t_epoll & epollVar, int i )
{
	std::string			line;
	std::stringstream 	ss;
	std::string			temp;
	size_t				pos;

	temp = this->_request.substr( this->_request.find( "\r\n\r\n", 0 ) + 4,  this->_request.size()- 1 );
	while ((pos = _request.find("\r")) != std::string::npos)
	  	_request.erase(pos, 1);
	ss << this->_request;
	while (!ss.eof())
	{
		std::getline(ss, line);
		_parseMethodAndPath(line);
//		_checkUserAgent( line );
		if ( this->_method == GET || this->_method == DELETE )
		{
			_parseHost(line);
			_parseAccept(line);
			_parseOrigin( line );
		}
		else if ( this->_method == POST )
		{
			_parseHost(line);
			_parseAccept(line);
			_parseOrigin( line );
			_parseContentLength( line );
			_parseContentType( line );
			_parseContentDisposition( line );
		}
	}
	if ( this->_method == POST )
	{
		if ( !this->_isSetContentLength || !this->_contentLength
			|| this->_sizeFile > this->_contentLength
			|| this->_contentLength > 2000000000 )
		{
			if ( !this->_isSetContentLength )
				setStatusError( 411, epollVar, i );
			else if ( !this->_contentLength || this->_sizeFile > this->_contentLength )
				setStatusError( 400, epollVar, i );
			else if ( this->_contentLength > 2000000000 )
				setStatusError( 413, epollVar, i );
			this->_isSetRequest = false;
			return ;
		}
			
		this->_request = temp;
		if ( this->_contentType == "application/x-www-form-urlencoded" )
			this->_queryString = this->_request;
	}
	if ( this->_method == GET || this->_method == DELETE
			|| this->_method == BAD_REQUEST 
			|| ( this->_method == POST && this->_request.size() == this->_contentLength
				&& this->_boundary.empty()))
	{
		changeEpollEvent( epollVar, i );
		this->_isSetRequest = false;
	}
}

int			Request::readData( int readFd, size_t bufferSize, int flag,
				t_epoll & epollVar, int i )
{
	char	bufferRead[ bufferSize ];
	int		rd = 0;

	if ( (rd = recv( readFd , bufferRead, bufferSize, 0 )) <= 0 )
	{
		if ( rd < 0 )
			std::cout << RED << "Connexion client lost qwd" << SET << std::endl;
		else
			std::cout << RED << "Connexion client is closed" << SET << std::endl;
		epoll_ctl(epollVar.epollFd, EPOLL_CTL_DEL, epollVar.events[i].data.fd, NULL);
		close( epollVar.events[i].data.fd );	
		return ( rd );
	}

	bufferRead[rd] = '\0';
	this->_sizeFile += rd;
	this->_request += bufferRead;
	if ( !this->_isSetRequest && this->_request.find( "\r\n\r\n", 0 ) == std::string::npos )
	{
		setStatusError( 431, epollVar, i );
		return ( rd );
	}
	else if ( !this->_isSetRequest && this->_request.find( "\r\n\r\n", 0 ) != std::string::npos )
	{
		this->_sizeFile -= (this->_request.find( "\r\n\r\n", 0 ) + 4);
		this->_isSetRequest = true;
	}
	if ( flag && this->_sizeFile == this->_contentLength )
		changeEpollEvent( epollVar, i );
	if ( (size_t)rd < bufferSize && this->_sizeFile < this->_contentLength
		&& this->_isSetContentLength )
		setStatusError( 400, epollVar, i );
	std::cout << GREEN <<  "Server side receive from client : \n" << this->_request << SET << std::endl;

	return ( rd );
}


int 	Request::getStatus(void) const 
{
	return (_status);
}