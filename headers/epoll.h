/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epoll.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 12:55:28 by amahla            #+#    #+#             */
/*   Updated: 2022/10/24 13:00:17 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __EPOLL_H__
# define __EPOLL_H__

# define MAXFD 5000

# include <sys/epoll.h>

typedef struct s_epoll
{
	int					maxNbFd;
	int					epollFd;
	struct epoll_event	new_event;
	struct epoll_event	events[MAXFD];
}	t_epoll;

typedef typename std::vector<Client>::iterator	itClient;
typedef typename std::vector<Server>::iterator	itServer;

			/*	serv_process_epoll_utils.cpp	*/
itClient	find( std::vector<Client> & clients, int clientFd );
itServer	find( std::vector<Server> & servers, int serverFd );
void		setEpollQueue( t_epoll & epollVar, std::vector<Server> & servers );

			/*	serv_process_epoll_loop	*/
void		servProcess( std::vector<Server> & servers,
				std::vector<Client> & clients, t_epoll & epollVar );

#endif
