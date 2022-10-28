/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 18:54:23 by amahla            #+#    #+#             */
/*   Updated: 2022/10/27 19:21:55 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __WEBSERV_H__
# define __WEBSERV_H__

# include "WebServException.hpp"
# include "Server.hpp"
# include "Client.hpp"
# include <vector>
# include "ParseFile.hpp"

#define MAX_PORT 65535
#define MIN_PORT 1024

			/*	socket_settings.cpp		*/
void		nonBlockSock( int & servSock );
void		setServerSockets( std::vector<Server> & servers );

			/*	weserv.cpp				*/
void		webServ( const char *av );

			/*	serv_process_epoll.cpp	*/
void		appServ( std::vector<Server> & servers );

			/*	serv_process_epoll_utils.cpp	*/
void		signal_handler(int sig);
Server		* isServer( std::vector<Server> & servers, int fd );


#endif
