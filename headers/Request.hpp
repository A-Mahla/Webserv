/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:46:05 by amahla            #+#    #+#             */
/*   Updated: 2022/11/02 15:49:01 by slahlou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __REQUEST_HPP__
# define __REQUEST_HPP__

#include <vector>
#include <iostream>
#include <sstream>
# include <cstdlib>
# include <map>

enum e_method
{
    BAD_REQUEST,
    GET,
    POST,
    DELETE
};

class Request
{

	private:

		std::string					_request;

		bool						_isSetRequest;
		bool						_isSetBoundary;

		size_t						_contentLength;
		std::string					_contentLenghtStr;
		std::string					_contentType;
		std::string					_boundary;
		std::string					_origin;
		e_method					_method;
		std::string					_path;
		std::string					_port;
		std::string					_addr;
		std::vector<std::string>	_accept;

		std::map< std::string, std::string >	_contentDisposition;


		//max
		void	_parseMethodAndPath(std::string request);
		void	_parseAccept( std::string request );
		void	_parseHost( std::string str_const );
		bool	_getPath(std::string request);

		//amir
		void		_parseOrigin( std::string request );
		void		_parseContentLength( std::string request );
		void		_parseContentType( std::string request );
		void		_parseContentDisposition( std::string request );


	public:

		Request( void );
	//	Request( itClient it, int readFd );
		Request( const Request & rhs );

		~Request( void );

		Request &	operator=( const Request & rhs );

		std::string			& getStringRequest( void );
		const std::string	& getStringRequest( void ) const;


		//max
		int				getMethode() const;
		std::string		getPath() const;
		std::string		getPort() const;
		std::string		getAddr() const;

		size_t			getContentLength( void ) const;
		std::string const & getContentLengthStr( void ) const;
		std::string		getContentType( void ) const;
		std::string		getBoundary( void ) const;
		std::string		getOrigin( void ) const;

		std::map< std::string, std::string >	& getContentDisposition( void );

		void		parseRequest(void);
};


#endif
