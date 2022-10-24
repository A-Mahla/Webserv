/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxenceeudier <maxenceeudier@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 18:54:23 by amahla            #+#    #+#             */
/*   Updated: 2022/10/24 11:36:40 by maxenceeudi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __WEBSERV_H__
# define __WEBSERV_H__

# include "WebServException.hpp"
# include "Server.hpp"
# include "Client.hpp"
//# include <sys/epoll.h>
# include <vector>

# define MAXFD 5000

/*typedef struct s_epoll
{
	int					maxNbFd;
	int					epollFd;
	struct epoll_event	new_event;
	struct epoll_event	events[MAXFD];
}	t_epoll;*/

typedef  std::vector<Client>::iterator	itClient;
typedef  std::vector<Server>::iterator	itServer;

			/*	socket_settings.cpp		*/
void		nonBlockSock( int & servSock );
void		setServerSockets( std::vector<Server> & servers );

			/*	weserv.cpp				*/
void		webServ( void );

			/*	serv_process_epoll.cpp	*/
void		appServ( std::vector<Server> & servers );

			/*	serv_process_epoll_utils.cpp	*/
itClient	find( std::vector<Client> & clients, int clientFd );
itServer	find( std::vector<Server> & servers, int serverFd );
//void		setEpollQueue( t_epoll & epollVar, std::vector<Server> & servers );
void		signal_handler(int sig);
int			isServer( std::vector<Server> & servers, int fd );

			/*	serv_process_epoll_loop	*/
//void		servProcess( std::vector<Server> & servers,
//				std::vector<Client> & clients, t_epoll & epollVar );

#endif
