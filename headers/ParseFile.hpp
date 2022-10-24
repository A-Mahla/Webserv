/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseFile.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 17:38:07 by amahla            #+#    #+#             */
/*   Updated: 2022/10/24 19:19:45 by amahla           ###   ########.fr       */
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
		bool				(Server::*ft[9])( std::string, Server );

	public:

		ParseFile( const char *argv );
		ParseFile( const ParseFile & rhs );

		~ParseFile( void );

		ParseFile &	operator=( const ParseFile & rhs );

		const std::vector<Server>	& getServers( void ) const;
		std::vector<Server>	& getServers( void );

		void	readFile( const char *file );

};


#endif
