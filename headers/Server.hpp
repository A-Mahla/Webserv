/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxenceeudier <maxenceeudier@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:46:05 by amahla            #+#    #+#             */
/*   Updated: 2022/10/24 14:37:16 by maxenceeudi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_HPP__
# define __SERVER_HPP__

class Server
{

	private:

		int	_servSock;
		int	_port;

	public:

		Server( void );
		Server( const Server & rhs );
		Server(int port) : _port(port){};

		~Server( void );

		Server &	operator=( const Server & rhs );

		int	&		getSock( void );
		const int	&	getSock( void ) const;
		const int	&	getPort( void ) const;
		const int	&	getIp( void ) const;

};


#endif
