/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:46:05 by amahla            #+#    #+#             */
/*   Updated: 2022/10/25 14:45:33 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include <map>
# include <vector>

class Server
{

	private:

		std::vector< std::string >		_serverName;
		int								_servSock;
		int								_port;
		std::map< std::string, bool >	_is_set;

	public:

		Server( void );
		Server( const Server & rhs );

		~Server( void );

		Server &	operator=( const Server & rhs );

		std::map< std::string, bool >	& getMap( void );

		int	&						getSock( void );
		const int					& getSock( void ) const;
		std::vector< std::string >	& getServerName( void );
		const int					& getPort( void ) const;
		const int					& getIp( void ) const;

		void				setServerName( std::vector< std::string > serverName );

};


#endif
