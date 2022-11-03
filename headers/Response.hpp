/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meudier <meudier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:46:05 by amahla            #+#    #+#             */
/*   Updated: 2022/11/03 13:07:40 by meudier          ###   ########.fr       */
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
		void		_clear_env(char **env);
		void    	_execCGI(char **env);
		std::string _getEnv(std::string key, char **env);
		std::string	_readFile(std::string path);
		std::string	_readFile(std::string path, Server &serv);
		void		_get_good_Root(std::string path, Server *serv);
		void		_initVar(std::string *var, Request const & req, Server const & serv);
		void		_printErrorPage(void);
		void		_getErrorPage(void);

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
		
		

		void				GET_response(Server &serv, Request &req);
		void				POST_response(Server &serv, Request &req);
		/*----------BUILDING CGI ENVIRONNEMENT---------------------*/
		char**				buildCGIenv(Request const & req, Server const & serv);
		char**				ft_split(char const *s, char c);

};

char	*ft_find_wrd(char const *s, char set, int wordneeded);
char	*ft_get_wrd(char const *s, int e);
int		ft_count_wrd(char const *s, char c);
int		ft_o(char c1, char c2);

#endif
