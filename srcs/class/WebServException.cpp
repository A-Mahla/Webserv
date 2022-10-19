/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServException.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 21:13:53 by amahla            #+#    #+#             */
/*   Updated: 2022/10/19 15:18:19 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServException.hpp"
#include <iostream>

WebServException::WebServException( const std::string ftName, const std::string sysCallName,
			const std::string error, const std::string fileName ) :
			_ftName(ftName), _sysCallName(sysCallName), _error(error), _fileName(fileName)
{
	if ( DEBUG )
		std::cout << "WebServException Default Constructor" << std::endl;

	this->_errorMessage = this->_ftName + ":	";
	this->_errorMessage += this->_sysCallName + ":	";
	this->_errorMessage += this->_error;
	this->_errorMessage += "	( " + this->_fileName + " )\n";
}

WebServException::WebServException( const std::string ftName, const std::string error,
	const std::string fileName ) :
		_ftName(ftName), _error(error), _fileName(fileName)
{
	if ( DEBUG )
		std::cout << "WebServException Default Constructor" << std::endl;

	this->_errorMessage = this->_ftName + ":	";
	this->_errorMessage += this->_error;
	this->_errorMessage += "	( " + this->_fileName + " )\n";
}

const char*	WebServException::what( void ) const throw()
{
	return ( this->_errorMessage.c_str() );
}
