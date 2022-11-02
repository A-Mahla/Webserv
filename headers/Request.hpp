/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:46:05 by amahla            #+#    #+#             */
/*   Updated: 2022/11/02 12:40:28 by slahlou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __REQUEST_HPP__
# define __REQUEST_HPP__

#include <vector>
#include <iostream>
#include <sstream>

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
		e_method					_method;
		std::string					_path;
		std::string					_port;
		std::string					_addr;
		std::vector<std::string>	_accept;


		//max
		void	_parseMethodAndPath(std::string request);
		void	_parseAccept( std::string request );
		void	_parseHost( std::string str_const );
		bool	_getPath(std::string request);


	public:

		Request( void );
		Request( const Request & rhs );

		~Request( void );

		Request &	operator=( const Request & rhs );

		std::string			& getStringRequest( void );
		const std::string	& getStringRequest( void ) const;


		//max
		int			getMethode() const;
		std::string	getPath() const;
		std::string	getPort() const;
		std::string	getAddr() const;

		void		parseRequest(void);
};


#endif
