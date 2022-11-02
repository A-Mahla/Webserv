/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:46:05 by amahla            #+#    #+#             */
/*   Updated: 2022/11/02 12:30:23 by slahlou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__
# include "Request.hpp"
#include "Server.hpp"

# include "Server.hpp"
# include "Request.hpp"

class Response
{

	private:

		std::string	_response;
		Server		*_server;
		int			_status;

		void		_initVar(std::string *var, Request const & req, Server const & serv);
	public:

		Response( void );
		Response( Server & serv, Request & req);
		Response( const Response & rhs );

		~Response( void );

		Response &	operator=( const Response & rhs );

		std::string			& getStringResponse( void );
		const std::string	& getStringResponse( void ) const;
		Server				* getServer(void);
		int					& getStatus(void);
		void				  get_good_Root(std::string path, Server *serv);
		std::string			  readFile(std::string path, Server &serv);

		void				GET_response(Server &serv, Request &req);
		/*----------BUILDING CGI ENVIRONNEMENT---------------------*/
		char**				buildCGIenv(Request & req, Server & serv);
		char**				ft_split(char const *s, char c);

};

char	*ft_find_wrd(char const *s, char set, int wordneeded);
char	*ft_get_wrd(char const *s, int e);
int		ft_count_wrd(char const *s, char c);
int		ft_o(char c1, char c2);

#endif
