/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseFile.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amahla <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 17:41:45 by amahla            #+#    #+#             */
/*   Updated: 2022/10/26 01:35:09 by amahla           ###   ########.fr       */
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

	this->_ft[0] = &ParseFile::setServerName;
	this->readFile( av );
	// reset les location Server en fonction du server parent
	// resetLocation();
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

void	ParseFile::readContent( std::ifstream & ifs, std::string temp, const std::string file, Server *parent )
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
			{
				if ( !parent )
					this->_servers.push_back( server );
				else
					parent->getLocation()[ file ] = server;
				break ;
			}
		}

		(this->*_ft[0])( temp.c_str() + i, server );
		setLocation( ifs, temp.c_str() + i, server );

/*		for ( i = 0; i < 9 && (*ft[i])( temp.c_str() + i, server ); i++ )
			;
		if ( i == 9 )
			throw WebServException( "ParseFile.cpp", "readFile", "Invalid format config file" );
*/	}
}

void	ParseFile::FormatFile( std::ifstream & ifs, std::string temp )
{
	int	i;

	do 
	{
		i = 0;
		i += whileSpace( temp.c_str() + i );
		if ( !temp[i] )
			continue ;
		if ( temp[i] && temp.compare( i, 6, "server" ) != 0 )
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
					readContent( ifs, temp.c_str() + i, "", NULL );
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
	FormatFile( ifs, temp );

	ifs.close();
}

bool	ParseFile::setServerName( const std::string str, Server & server )
{
	std::string	temp;
	int			i = 0;

	if ( server.getMap()["server_name"] == true )
		return ( false );
	if ( str.compare( i, 11, "server_name" ) == 0 )
	{
		i += 11;
		while ( str[i] && str[i] != ';' )
		{
			i += whileSpace( str.c_str() + i );
			if ( str.compare( i, 3, "\"\"" ) == 0 ) 
				i += 2;
			if ( str[i] == ';' )
				break ;
			while ( str[i] && !( str[i] == ' ' || str[i] == '\t' || str[i] == ';' ) )
				temp.push_back( str[i++] );
			server.getServerName().push_back( temp );
			temp.clear();
		}
		i = i + whileSpace( str.c_str() + i ) + 1;
		if ( str[i] )
				return ( false );
	}
	else
		return ( false );

	server.getMap()["server_name"] = true;
	return ( true );
}	

bool	ParseFile::setLocation( std::ifstream & ifs, std::string temp, Server & server )
{
	std::string	file;
	int			i = 0;
	
	try
	{
		if ( temp.compare( i, 8, "location" ) != 0 )
			throw WebServException( "ParseFile.cpp", "setLocation", "Invalid format config file" );
		i += 8;
		i += whileSpace( temp.c_str() + i );
		while ( temp[i] >= 33 && temp[i] <= 126 )
			file.push_back( temp[i++] );
		i += whileSpace( temp.c_str() + i );
		if ( temp[i++] == '{' )
		{
			i += whileSpace( temp.c_str() + i );
			if ( temp[i] || file.empty() )
				throw WebServException( "ParseFile.cpp", "setLocation", "Invalid format config file" );
			else
				readContent( ifs, temp.c_str() + i, file, &server );
		}
		else
			throw WebServException( "ParseFile.cpp", "setLocation", "Invalid format config file" );
	}
	catch ( std::exception & e )
	{
		return ( false );
	}
	return ( true );
}
