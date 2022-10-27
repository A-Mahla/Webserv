/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxenceeudier <maxenceeudier@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:46:05 by amahla            #+#    #+#             */
/*   Updated: 2022/10/27 11:17:55 by maxenceeudi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__

# include "Request.hpp"
# include "Response.hpp"

class Client
{

	private:

		int			_clientSock;
		Request		_request;
		Response	_response;
		Server		_server;

	public:

		Client( void );
		Client( const int socket );
		Client( const Client & rhs );

		~Client( void );

		Client &	operator=( const Client & rhs );

		int				& getSock( void );
		const int		& getSock( void ) const;
		Request			& getRequest( void );
		const Request	& getRequest( void ) const;
		Response		& getResponse( void );
		const Response	& getResponse( void ) const;

		/*=========================*/
		/*       max test          */
		/*=========================*/
		Response		&getResponse( Server serv, Request req);
		Server			&getServer(void);
		Client(const int socket, const Server serv);

};


#endif
