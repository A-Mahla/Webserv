/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:46:05 by amahla            #+#    #+#             */
/*   Updated: 2022/10/22 17:03:36 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__

# include "Request.hpp"
# include "Response.hpp"

class Client
{

	private:

		int			_clientSock;
		Request		_request;
		Response	_response;

	public:

		Client( void );
		Client( const int socket );
		Client( const Client & rhs );

		~Client( void );

		Client &	operator=( const Client & rhs );

		int				& getSock( void );
		const int		& getSock( void ) const;
		Request			& getRequest( void );
		const Request	& getRequest( void ) const;
		Response		& getResponse( void );
		const Response	& getResponse( void ) const;

};


#endif