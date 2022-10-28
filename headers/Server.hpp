/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:46:05 by amahla            #+#    #+#             */
/*   Updated: 2022/10/28 12:35:06 by slahlou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include <map>
# include <vector>
# include <netinet/in.h>

class Server
{

	private:

		std::vector< std::string >			_serverName;
		int									_servSock;
		int									_port;
		int									_addr;
		int									_clientBody;
		bool								_autoindex;
		std::string							_root;
		std::vector< std::string >			_index;
		std::map< std::string, std::string>	_error_pages;
		std::map< std::string, bool >		_is_set;
		std::map< std::string, Server >		_location;

	public:

		Server( void );
		Server( const Server & rhs );

		~Server( void );

		Server &	operator=( const Server & rhs );

		std::map< std::string, bool >	& getMap( void );
		bool				get_is_set( std::string attribute );
		void				set_is_set( std::string attribute );

		int	&									getSock( void );
		const int								& getSock( void ) const;
		std::vector< std::string >				& getServerName( void );
		std::map< std::string, std::string >	& get_error_pages(void);
		std::map< std::string, Server >			& getLocation( void );
		const int								& getPort( void ) const;
		const int								& getAddr( void ) const;
		int										get_clientBody( void ) const;
		std::string								get_root( void ) const;
		std::vector< std::string >				& get_index( void );
		bool									getAutoindex( void ) const;

		void									setAutoindex( bool onOff );
		void									set_clientBody( const int val );
		void									set_root( const std::string root );
		void									setSock( const int sock );
		void									setAddr( const int addr );
		void									setPort( const int port );

};

std::ostream	& operator<<( std::ostream & ifs, Server rhs );

#endif
