/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 21:10:19 by amahla            #+#    #+#             */
/*   Updated: 2022/10/21 22:24:32 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "webserv.h"
# include <iostream>
# include <cstdlib>
# include "WebServException.hpp"

int main( void )
{
	try
	{
		webServ();
	}
	catch ( std::exception & e )
	{
		std::cout << e.what() << std::endl;
		return ( EXIT_FAILURE );
	}
	return ( EXIT_SUCCESS );
}
