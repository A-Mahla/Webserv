/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 12:45:35 by amahla            #+#    #+#             */
/*   Updated: 2022/10/18 14:56:52 by amahla           ###   ########.fr       */
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

	int	addrlen = sizeof(address);
	int			new_socket;
	char		buffer_read[1024];
	const char	*buffer_write = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	int			n;

		
	std::cout << std::endl << "-------------- Server is on ---------------" << std::endl << std::endl;
	while (1)
	{

		if ( ( new_socket = accept( socket_fd, reinterpret_cast< struct sockaddr *>(&address),
			reinterpret_cast< socklen_t * >(&addrlen) ) ) < 0 )
		{}

		if ( new_socket >= 0 )
		{
			while (1)
			{
				if ( (n = recv( new_socket, buffer_read, 1023, 0 )) <= 0 )
					break ;

				buffer_read[n] = '\0';
				std::cout << "Server side receive from client : " << buffer_read << std::endl;

				if ( send( new_socket, buffer_write, strlen(buffer_write), 0 ) <= 0 )
					break ;
			}

			close( new_socket );
		}
	}


	return 0;
}
























