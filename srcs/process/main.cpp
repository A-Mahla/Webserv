/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 21:10:19 by amahla            #+#    #+#             */
/*   Updated: 2022/10/25 15:22:45 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "webserv.h"
# include <iostream>
# include <cstdlib>
# include "WebServException.hpp"

int main( int ac, char **av )
{
	if ( ac != 2 )
	{
		std::cout << "Error: Config file needed as argument" << std::endl;
		return ( EXIT_FAILURE );
	}
	try
	{
		webServ( av[1] );
	}
	catch ( std::exception & e )
	{
		std::cout << RED << e.what() << std::endl << SET;
		return ( EXIT_FAILURE );
	}
	return ( EXIT_SUCCESS );
}
