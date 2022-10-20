/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServException.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 21:22:46 by amahla            #+#    #+#             */
/*   Updated: 2022/10/20 20:57:49 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __WEBSERVEXCEPTION_HPP__
# define __WEBSERVEXCEPTION_HPP__

# include <stdexcept>
# include <string>
# include "defines.h"

class WebServException : public std::exception
{

	private:

		std::string	_ftName;
		std::string	_error;
		std::string	_fileName;
		std::string _errorMessage;

	public:

		WebServException( const std::string _fileName, const std::string _ftName, const std::string _error );
		virtual ~WebServException( void ) throw();

		virtual const char*	what( void ) const throw();
		

};


#endif
