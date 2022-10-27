/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:51:31 by amahla            #+#    #+#             */
/*   Updated: 2022/10/26 10:39:32 by slahlou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
#define SERVER_H

class Server
{

	private:

		int	_servSock;
		int	_port;
		int	_addr;

	public:

		Server( void );
		Server( const int port );
		Server( const Server & rhs );

		~Server( void );

		Server &	operator=( const Server & rhs );

		int	&		getSock( void );
		const int	&	getSock( void ) const;
		const int	&	getPort( void ) const;
		const int	&	getAddr( void ) const;
		void			setAddr( int addr );
		void			setPort( int port );
		void			setSock( int sock );
		void			print(void);

};

#endif
