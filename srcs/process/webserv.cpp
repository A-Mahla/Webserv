/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meudier <meudier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 12:45:35 by amahla            #+#    #+#             */
/*   Updated: 2022/11/10 14:48:17 by meudier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "webserv.h"
// ==== test =====
# include <iostream>
// ==============

void	webServ( const char *av )
{
	ParseFile	file( av );
	std::vector<Server> servers = file.getServers();
	servers = file.getServers();
	setServerSockets( file.getServers() );
	appServ( file.getServers() );
}
