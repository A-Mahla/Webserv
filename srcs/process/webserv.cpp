/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meudier <meudier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 12:45:35 by amahla            #+#    #+#             */
/*   Updated: 2022/10/31 08:46:06 by meudier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "webserv.h"
// ==== test =====
# include <iostream>
// ==============

void	webServ( const char *av )
{
	ParseFile	file( av );
// ========= test ==========
	
	std::vector<Server> servers = file.getServers();

	servers = file.getServers();
	for ( std::size_t i(0); i < servers.size(); i++ )
		std::cout << std::endl << servers[i] << std::endl;
// ========================= 

	setServerSockets( file.getServers() );
	appServ( file.getServers() );

}
