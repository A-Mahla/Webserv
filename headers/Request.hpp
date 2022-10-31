/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meudier <meudier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:46:05 by amahla            #+#    #+#             */
/*   Updated: 2022/10/31 17:57:13 by meudier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __REQUEST_HPP__
# define __REQUEST_HPP__

class Request
{

	private:

		std::string	_request;
		e_method	_method;
		std::string	_path;
		std::string	_port;
		std::string	_addr;


		//max
		bool	_parseMethod(std::string &request);
		bool	_parseHost( const std::string str_const );


	public:

		Request( void );
		Request( const Request & rhs );

		~Request( void );

		Request &	operator=( const Request & rhs );

		std::string			& getStringRequest( void );
		const std::string	& getStringRequest( void ) const;


		//max
		int			getMethode();
		std::string	getPath();
		std::string	getPort();
		std::string	getAddr();

		void		parseRequest(void);
};


#endif
