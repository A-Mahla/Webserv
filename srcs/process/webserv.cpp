/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxenceeudier <maxenceeudier@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 12:45:35 by amahla            #+#    #+#             */
/*   Updated: 2022/10/24 14:37:49 by maxenceeudi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "webserv.h"

void	webServ( void )
{
	std::vector<Server> servers;

	servers.push_back( Server(8080) );
	servers.push_back( Server(4242) );

	setServerSockets( servers );
	appServ( servers );

}
