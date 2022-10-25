/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxenceeudier <maxenceeudier@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 18:54:23 by amahla            #+#    #+#             */
/*   Updated: 2022/10/25 16:10:15 by maxenceeudi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __WEBSERV_H__
# define __WEBSERV_H__

# include "WebServException.hpp"
# include "Server.hpp"
# include "Client.hpp"
# include <vector>


			/*	socket_settings.cpp		*/
void		nonBlockSock( int & servSock );
void		setServerSockets( std::vector<Server> & servers );

			/*	weserv.cpp				*/
void		webServ( void );

			/*	serv_process_epoll.cpp	*/
void		appServ( std::vector<Server> & servers );

			/*	serv_process_epoll_utils.cpp	*/
void		signal_handler(int sig);
int			isServer( std::vector<Server> & servers, int fd );

#endif
