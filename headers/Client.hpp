/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxenceeudier <maxenceeudier@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:46:05 by amahla            #+#    #+#             */
/*   Updated: 2022/11/04 09:43:16 by maxenceeudi      ###   ########.fr       */
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
		int						_readStatus;
		std::vector< Server * >	_serverList;
		Server					* _server;
		Request					_request;
		Response				_response;
		void					_chooseServer(void);

		void	_get_good_Root(std::string path, Server *serv);
		void	_chooseServer( std::string path );

	public:

		Client( void );
		Client( const int socket );
		Client( const Client & rhs );

		~Client( void );

		Client &	operator=( const Client & rhs );

		
		int						& getSock( void );
		const int				& getSock( void ) const;
		int						getReadStatus( void );
		Request					& getRequest( void );
		const Request			& getRequest( void ) const;
		void					setRequest( t_epoll & epollVar, int i );
		Response				& getResponse( void );
		Response				&getResponse( Server & serv, Request & req, int fd);
		const Response			& getResponse( void ) const;
		Server					* getServer( void );
		//Server					&getServer( void );
		std::vector< Server * >	& getServerList( void );


};


#endif
