/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_client.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 12:45:35 by amahla            #+#    #+#             */
/*   Updated: 2022/10/20 20:39:19 by amahla           ###   ########.fr       */
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

	struct sockaddr_in	address;
	const int			port = 8080;
	memset( (char *)&address, 0, sizeof(address) );
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl( INADDR_ANY );
	address.sin_port = htons( port );
	int	addrlen = sizeof(address);

	if ( connect( socket_fd, reinterpret_cast< struct sockaddr *>(&address), addrlen ) < 0 )
	{
		std::cout << "connect() failed" << std::endl;
		exit(1);
	}

	std::cout << "Client is connect..." << std::endl;

	char		buffer_read[1024];
	const char	*buffer_write = "Hello from client";
	int			n;

		
	int	i = 10;
	while ( i-- )
	{

		usleep( 2000000 );
		if ( send( socket_fd, buffer_write, strlen(buffer_write), 0 ) < 0 )
		{
			std::cout << "send() server failed" << std::endl;
			exit(1);
		}

		if ( (n = recv( socket_fd, buffer_read, 1023, 0 )) < 0 )
		{
			std::cout << "recv() server failed" << std::endl;
			exit(1);
		}
		buffer_read[n] = '\0';

		std::cout << "Client side receive from server : " << buffer_read << std::endl;
			

	}
	close(socket_fd);

	return 0;
}
























