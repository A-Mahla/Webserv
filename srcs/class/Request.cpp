/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:51:31 by amahla            #+#    #+#             */
/*   Updated: 2022/10/22 17:29:13 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include <iostream>
#include "Request.hpp"

Request::Request( void )
{
	if ( DEBUG )
		std::cout << "Request Default Constructor" << std::endl;
}

Request::Request( const Request & rhs )
{
	if ( DEBUG )
		std::cout << "Request copy Constructor" << std::endl;
	*this = rhs;
}

Request::~Request( void )
{
	if ( DEBUG )
		std::cout << "Request Default Destructor" << std::endl;
}

Request &	Request::operator=( const Request & rhs )
{
	if ( this != &rhs )
	{
		this->_request = rhs._request;
	}
	return ( *this );
}

std::string			& Request::getStringRequest( void )
{
	return ( this->_request );
}

const std::string	& Request::getStringRequest( void ) const
{
	return ( this->_request );
}

