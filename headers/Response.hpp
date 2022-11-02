/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meudier <meudier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:46:05 by amahla            #+#    #+#             */
/*   Updated: 2022/11/02 13:04:05 by meudier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__
# include "Request.hpp"
#include "Server.hpp"

# include "Server.hpp"
# include "Request.hpp"

# include <iostream>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <sstream>
# include <cstring>

class Response
{

	private:

		int			_fd;
		std::string	_response;
		Server		*_server;
		int			_status;
		char    	**_getArgv(std::string script);
		void    	_clear_argv(char **argv);
		void    	_execCGI(void);
		

	public:

		Response( void );
		Response( Server & serv, Request & req, int fd);
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
		void				POST_response(Server &serv, Request &req);
};


#endif
