/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseFile.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 17:41:45 by amahla            #+#    #+#             */
/*   Updated: 2022/10/24 21:13:53 by amahla           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include "ParseFile.hpp"
# include <fstream>
# include "WebServException.hpp"

ParseFile::ParseFile( const char *av )
{
	if ( DEBUG )
		std::cout << "ParseFile Default Constructor" << std::endl;
	
	this->readFile( av );
}

ParseFile::ParseFile( const ParseFile & rhs )
{
	if ( DEBUG )
		std::cout << "ParseFile copy Constructor" << std::endl;
	*this = rhs;
}

ParseFile::~ParseFile( void )
{
	if ( DEBUG )
		std::cout << "ParseFile Default Destructor" << std::endl;
}

ParseFile &	ParseFile::operator=( const ParseFile & rhs )
{
	if ( this != &rhs )
		this->_servers = rhs._servers;
	return ( *this );
}

std::vector<Server>	& ParseFile::getServers( void )
{
	return ( this->_servers );
}

const std::vector<Server>	& ParseFile::getServers( void ) const
{
	return ( this->_servers );
}

int	whileSpace( std::string temp )
{
	int	i = 0;

	while ( temp[i] == ' ' || temp[i] == '\t' )
		i++;
	return ( i );
}

void	readContent( std::ifstream & ifs, std::string temp )
{
	Server	server;
	int		i;

	while ( std::getline( ifs, temp ).good() )
	{
		i = 0;
		if ( !temp[i] )
			continue ;

		i += whileSpace( temp.c_str());
		if ( temp[i] == '}' )
		{
			i++;
			i += whileSpace( temp.c_str() + i);
			if ( temp[i] )
				throw WebServException( "ParseFile.cpp", "readFile", "Invalid format config file" );
			else
				break ;
		}

/*		for ( i = 0; i < 9 && (*ft[i])( temp.c_str() + i, server ); i++ )
			;
		if ( i < 9 )
			this->_servers.push_back( server );
		else
			throw WebServException( "ParseFile.cpp", "readFile", "Invalid format config file" );
*/	}
}

void	FormatBeginFile( std::ifstream & ifs, std::string temp )
{
	int	i;

	do 
	{
		i = 0;
		i += whileSpace( temp.c_str() + i );
		if ( !temp[i] )
			continue ;
		if ( ( temp[i] && temp.compare( i, 6, "server" ) != 0 ) )
			throw WebServException( "ParseFile.cpp", "readFile", "Invalid format config file" );
		else 
		{
			i += 6;
			i += whileSpace( temp.c_str() + i );
			if ( temp[i++] == '{' )
			{
				i += whileSpace( temp.c_str() + i );
				if ( temp[i] )
					throw WebServException( "ParseFile.cpp", "readFile", "Invalid format config file" );
				else
					readContent( ifs, temp.c_str() + i );
			}
			else
				throw WebServException( "ParseFile.cpp", "readFile", "Invalid format config file" );
		}
	} while ( std::getline( ifs, temp ).good() );
	if ( ifs.bad() )
		throw WebServException( "ParseFile.cpp", "readFile", "An error occurred while reading the config file" );
}


void	ParseFile::readFile( const char *file )
{
	std::ifstream		ifs( file, std::ifstream::in );
	std::string			temp;
	
	if ( !ifs.is_open() )
		throw WebServException( "ParseFile.cpp", "readFile", "File not found" );
	std::getline( ifs, temp );
	if ( ifs.eof() && temp == "")
		WebServException( "ParseFile.cpp", "readFile", "Empty config file" );
	FormatBeginFile( ifs, temp );

	ifs.close();
}

