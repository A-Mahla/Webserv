/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meudier <meudier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:46:05 by amahla            #+#    #+#             */
/*   Updated: 2022/11/01 17:58:05 by meudier          ###   ########.fr       */
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

		int						_clientSock;
		std::vector< Server * >	_serverList;
		Server					* _server;
		Request					_request;
		Response				_response;
		void					_chooseServer(void);

	public:

		Client( void );
		Client( const int socket );
		Client( const Client & rhs );

		~Client( void );

		Client &	operator=( const Client & rhs );

		
		int						& getSock( void );
		const int				& getSock( void ) const;
		Request					& getRequest( void );
		const Request			& getRequest( void ) const;
		void					setRequest(char * str);
		Response				& getResponse( void );
		Response				&getResponse( Server & serv, Request & req );
		const Response			& getResponse( void ) const;
		Server					* getServer( void );
		//Server					&getServer( void );
		std::vector< Server * >	& getServerList( void );

};


#endif
