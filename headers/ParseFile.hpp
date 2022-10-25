/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseFile.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 17:38:07 by amahla            #+#    #+#             */
/*   Updated: 2022/10/25 16:53:54 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PARSEFILE_HPP__
# define __PARSEFILE_HPP__

# include "webserv.h"
# include <string>

class ParseFile
{

	private:

		std::vector<Server>	_servers;
		bool				(ParseFile::*_ft[9])( const std::string, Server & );

	public:

		ParseFile( const char *argv );
		ParseFile( const ParseFile & rhs );

		~ParseFile( void );

		ParseFile &	operator=( const ParseFile & rhs );

		const std::vector<Server>	& getServers( void ) const;
		std::vector<Server>	& getServers( void );

		void	readContent( std::ifstream & ifs, std::string temp );
		void	FormatFile( std::ifstream & ifs, std::string temp );
		void	readFile( const char *file );

		bool	setServerName( const std::string str, Server & server );

};

#endif
