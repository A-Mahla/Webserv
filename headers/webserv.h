/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 18:54:23 by amahla            #+#    #+#             */
/*   Updated: 2022/10/24 19:22:25 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __WEBSERV_H__
# define __WEBSERV_H__

# include "WebServException.hpp"
# include "Server.hpp"
# include "Client.hpp"
# include <vector>
# include "ParseFile.hpp"


			/*	socket_settings.cpp		*/
void		nonBlockSock( int & servSock );
void		setServerSockets( std::vector<Server> & servers );

			/*	weserv.cpp				*/
void		webServ( const char *av );

			/*	serv_process_epoll.cpp	*/
void		appServ( std::vector<Server> & servers );

			/*	serv_process_epoll_utils.cpp	*/
void		signal_handler(int sig);
int			isServer( std::vector<Server> & servers, int fd );


#endif
