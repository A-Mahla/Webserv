/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_server_select.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 12:45:35 by amahla            #+#    #+#             */
/*   Updated: 2022/10/20 20:48:39 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include <string.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <fcntl.h>
# include <unistd.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/select.h>
# include <errno.h>
# include <vector>
# include <algorithm>


int main( void )
{
	int	socket_fd;

	if (( socket_fd = socket(AF_INET, SOCK_STREAM, 0 )) < 0 )
	{
		std::cout << "socket() failed" << std::endl;
		exit(1);
	}

	int	on = 1;

	if ( setsockopt( socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
		&on, sizeof(int) ) < 0 )
	{
		std::cout << "setsockopt() failed" << std::endl;
		exit(1);
	}

	int flag;

	if ( (flag = fcntl( socket_fd, F_GETFL )) < 0 )
	{
		std::cout << "fcntl() F_GETFL failed" << std::endl;
		exit(1);
	}

	if ( fcntl( socket_fd, F_SETFL, flag | O_NONBLOCK ) < 0 )
	{
		std::cout << "fcntl() F_SETFL failed" << std::endl;
		exit(1);
	}

	struct sockaddr_in	address;
	const int			port = 8080;
	memset( (char *)&address, 0, sizeof(address) );
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl( INADDR_ANY );
	address.sin_port = htons( port );
	struct timeval timeout;

	if ( bind( socket_fd, reinterpret_cast< struct sockaddr * >(&address),
		sizeof(address)) < 0 )
	{
		std::cout << "bind() failed" << std::endl;
		exit(1);
	}

	if ( listen( socket_fd, 5) < 0 )
	{
		std::cout << "listen() failed" << std::endl;
		exit(1);
	}
	
	fd_set	readfds;
	fd_set	readfds_ready;
	int		status = 0;
	std::vector<int>	new_sockets;

	FD_ZERO(&readfds);
	FD_SET(socket_fd, &readfds);
	

	int	addrlen = sizeof(address);
	int			new_socket;
	int			max_sock = socket_fd;
	char		buffer_read[1024];
	const char	*buffer_write = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	int			n;

		
	std::cout << std::endl << "-------------- Server is on ---------------" << std::endl << std::endl;
	while (1)
	{
		while (!status)
		{
			timeout.tv_sec = 1;
			timeout.tv_usec = 0;
			FD_ZERO(&readfds_ready);
			for (std::size_t i = 0; i < new_sockets.size(); i++ )
			{
				if ( new_sockets[i] > max_sock )
					max_sock = new_sockets[i];
				FD_SET(new_sockets[i], &readfds);
			}
			readfds_ready = readfds;
			if (( status = select( max_sock + 1, &readfds_ready, NULL, NULL, &timeout )) < 0 )
			{
				std::cout << "select() error" << std::endl;
				exit(1);
			}
		}
		if ( FD_ISSET( socket_fd, &readfds_ready ) )
		{
			if ( (( new_socket = accept( socket_fd, reinterpret_cast< struct sockaddr *>(&address),
							reinterpret_cast< socklen_t * >(&addrlen) ) ) < 0) )
			{
				if ( errno != EWOULDBLOCK )
				{
					std::cout << "accept() failed" << std::endl;
					exit(1);
				}
			}
			if (new_socket > 0)			
			{
				std::cout << "Connection accepted" << std::endl;
				FD_SET(new_socket, &readfds_ready);
				new_sockets.push_back(new_socket);
			}
		}
		for (std::size_t i = 0; i < new_sockets.size(); i++ )
		{
			if ( FD_ISSET( new_sockets[i], &readfds_ready ) )
			{
				if ( (n = recv( new_sockets[i], buffer_read, 1023, 0 )) < 0 )
				{
					std::cout <<  "recv() failed" << std::endl;
					exit(1);
				}
				else if ( n == 0)
				{
					std::vector<int>::iterator it = new_sockets.begin();
					std::vector<int>::iterator ite = new_sockets.end();
					it = find( it, ite, new_sockets[i]);
					close(*it);
					new_sockets.erase( it );
					std::cout << "Connexion client is closed" << std::endl;
				}
				else
				{
					buffer_read[n] = '\0';
					std::cout << "Server side receive from client : " << buffer_read << std::endl;
					if ( n && send( new_sockets[i], buffer_write, strlen(buffer_write), 0 ) < 0 )
					{
						std::cout << "send() failed" << std::endl;
						exit(1);
					}
				}
			}
		}
	}
	FD_CLR( socket_fd, &readfds );
	FD_CLR( socket_fd, &readfds_ready );
	for (std::vector<int>::iterator i = new_sockets.begin(); i != new_sockets.end(); i++ )
		close( *i );
	return 0;
}
