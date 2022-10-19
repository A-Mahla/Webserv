/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 21:10:19 by amahla            #+#    #+#             */
/*   Updated: 2022/10/19 15:18:09 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServException.hpp"
#include <iostream>

int main( void )
{
	try
	{
		throw WebServException( "main", "try", "error fatal", "main.cpp" );
	}
	catch ( std::exception & e )
	{
		std::cout << e.what();
	}
	return 0;
}
