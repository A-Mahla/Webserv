/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxenceeudier <maxenceeudier@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:46:05 by amahla            #+#    #+#             */
/*   Updated: 2022/10/26 17:37:25 by maxenceeudi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__
# include "Request.hpp"
#include "Server.hpp"

class Response
{

	private:

		std::string	_response;

	public:

		Response( void );
		Response(Server serv, Request req);
		Response( const Response & rhs );

		~Response( void );

		Response &	operator=( const Response & rhs );

		std::string			& getStringResponse( void );
		const std::string	& getStringResponse( void ) const;

};


#endif
