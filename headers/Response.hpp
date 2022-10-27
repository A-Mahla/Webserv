/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:46:05 by amahla            #+#    #+#             */
/*   Updated: 2022/10/27 19:12:13 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__

# include "Server.hpp"
# include "Request.hpp"

class Response
{

	private:

		std::string	_response;

	public:

		Response( void );
		Response( Server & serv, Request & req);
		Response( const Response & rhs );

		~Response( void );

		Response &	operator=( const Response & rhs );

		std::string			& getStringResponse( void );
		const std::string	& getStringResponse( void ) const;

};


#endif
