/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxenceeudier <maxenceeudier@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:46:05 by amahla            #+#    #+#             */
/*   Updated: 2022/10/25 16:58:37 by maxenceeudi      ###   ########.fr       */
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
		
		//for max test
		map<string, bool>	_is_set;
		map<string, string>	_error_pages;
		int					_clientBody;
		vector<string>		_index;

		
		

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

		//max test
		bool				get_is_set(string atribute);
		int					get_clientBody(void);
		map<string, string>	&get_error_pages(void);
		vector<string>		&get_index(void);

		void	set_clientBody(int val);
		void	set_is_set(string atribute);


};


#endif
