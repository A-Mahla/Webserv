/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:51:31 by amahla            #+#    #+#             */
/*   Updated: 2022/10/27 10:59:00 by slahlou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
#define SERVER_H

class Server
{

	private:

		int		_servSock;
		int		_port;
		int		_addr;
		bool	_autoindex;

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
		bool			getAutoindex(void) const;

		void			setAddr( int addr );
		void			setPort( int port );
		void			setSock( int sock );
		void			setAutoindex(bool onOff);
		void			print(void);

};

#endif
