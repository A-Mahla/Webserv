/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meudier <meudier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:46:05 by amahla            #+#    #+#             */
/*   Updated: 2022/11/01 08:46:00 by meudier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__
# include "Request.hpp"
#include "Server.hpp"

# include "Server.hpp"
# include "Request.hpp"

class Response
{

	private:

		std::string	_response;
		Server		*_server;
		int			_status;

	public:

		Response( void );
		Response( Server & serv, Request & req);
		Response( const Response & rhs );

		~Response( void );

		Response &	operator=( const Response & rhs );

		std::string			& getStringResponse( void );
		const std::string	& getStringResponse( void ) const;
		Server				* getServer(void);
		int					& getStatus(void);
		void				  get_good_Root(std::string path, Server *serv);
		std::string			  readFile(std::string path, Server &serv);

		void				GET_response(Server &serv, Request &req);

};


#endif
