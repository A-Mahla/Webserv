/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 12:45:35 by amahla            #+#    #+#             */
/*   Updated: 2022/10/24 19:25:24 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "webserv.h"

void	webServ( const char *av )
{
	std::vector<Server> servers;

	ParseFile	file( av );
/*
	servers.push_back( Server() );

	setServerSockets( servers );
	appServ( servers );
*/
}
