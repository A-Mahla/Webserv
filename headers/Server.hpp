/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxenceeudier <maxenceeudier@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:46:05 by amahla            #+#    #+#             */
/*   Updated: 2022/10/26 12:19:20 by maxenceeudi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_HPP__
# define __SERVER_HPP__
# include <map>
# include <string>
# include <vector>

using namespace std;

class Server
{

	private:

		int	_servSock;
		int	_port;
		
		/*================================*/
		/*     max atributes              */
		/*================================*/
		map<string, bool>	_is_set;
		map<string, string>	_error_pages;
		int					_clientBody;
		vector<string>		_index;
		string				_root;
		/*=================================*/

		
		

	public:

		Server( void );
		Server( const int port );
		Server( const Server & rhs );

		~Server( void );

		Server &	operator=( const Server & rhs );

		int	&		getSock( void );
		const int	&	getSock( void ) const;
		const int	&	getPort( void ) const;
		const int	&	getIp( void ) const;

		/*================================*/
		/*     max methodes	              */
		/*================================*/
		bool				get_is_set(string atribute);
		int					get_clientBody(void);
		string				get_root(void);
		map<string, string>	&get_error_pages(void);
		vector<string>		&get_index(void);
		void				set_clientBody(int val);
		void				set_is_set(string atribute);
		void				set_root(string root);
		/*==================================*/


};


#endif
