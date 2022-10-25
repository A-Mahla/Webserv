/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 12:45:35 by amahla            #+#    #+#             */
/*   Updated: 2022/10/25 16:03:50 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "webserv.h"
//	test
# include <iostream>
// =======

void	webServ( const char *av )
{
	std::vector<Server> servers;
// ========= test ==========
	ParseFile	file( av );
	
	servers = file.getServers();
	Server test = servers[0];
//	for ( std::size_t i(0); i < test.getServerName().size(); i++ )
//		std::cout << test.getServerName()[i] << std::endl;
// =========================
/*
	servers.push_back( Server() );

	setServerSockets( servers );
	appServ( servers );
*/
}
