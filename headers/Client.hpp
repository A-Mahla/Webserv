/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meudier <meudier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:46:05 by amahla            #+#    #+#             */
/*   Updated: 2022/10/31 18:03:13 by meudier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__

# include "Request.hpp"
# include "Response.hpp"
# include "Server.hpp"

class Client
{
	private:

		int			_clientSock;
		Server		_server;
		Request		_request;
		Response	_response;

	public:

		Client( void );
		Client( const int socket );
		Client( const int socket, const Server & serv );
		Client( const Client & rhs );

		~Client( void );

		Client &	operator=( const Client & rhs );

		int				& getSock( void );
		const int		& getSock( void ) const;
		Request			& getRequest( void );
		const Request	& getRequest( void ) const;
		Response		& getResponse( void );
		Response		&getResponse( Server & serv, Request & req );
		const Response	& getResponse( void ) const;
		Server			&getServer( void );
		void			setRequest(char * str);

};


#endif
