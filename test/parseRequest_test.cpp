#include <iostream>
#include <string>
#include <sstream>
#include <vector>

enum e_method
{
	BAD_REQUEST,
	GET,
	POST,
	DELETE
};

int _method =  0;
std::string _port, _addr, _path;
std::vector<std::string> _accept;


void	_parseHost( const std::string request )
{
	if ( !request.compare(0, 6, "Host: ") )
	{
			_addr = request.substr(6, request.find(":",6) - 6);
			_port = request.substr((request.find(":", 6) + 1), ( (request.find("\0", 0)) - (request.find(":", 0) + 1) ));
			std::cout << "add = " << _addr << " port " << _port << "\n";
	}
}

void	_parseAccept( const std::string request )
{
	size_t  pos = 0;
	size_t  posTmp = 0;


	if ( !request.compare(0, 8, "Accept: ") )
	{
		posTmp = 8;
		while ((pos = request.find(",", pos)) != std::string::npos)
		{
			_accept.push_back(request.substr(posTmp, pos - posTmp));
			pos++;
			posTmp = pos;
		}
	}
}

bool    getPath(std::string request)
{
	size_t	pos = 0;
	if ((pos = request.find("HTTP/1.1")) != std::string::npos)
	{
		_path = request.substr( 0, request.find(" ", 0) );
		return (true);
	}
	return (false);
}

void		_parseMethodAndPath(std::string request)
{
	size_t pos = 0;

	if ((pos = request.find("GET")) != std::string::npos)
	{
		if (getPath(request.substr(4, request.find("\0", 0) - 4)))
			_method = GET;
	}
	else if ((pos = request.find("POST")) != std::string::npos)
	{
		if (getPath(request.substr(5, request.find("\0", 0) - 5)))
			_method = POST;
	}
	else if ((pos = request.find("DELETE")) != std::string::npos)
	{
		if (getPath(request.substr(7, request.find("\0", 0) - 7)))
			_method = DELETE;
	}
}

# include <cstdlib>
# include <map>

size_t									_contentLength = 0;
std::string								_contentType;
std::string								_boundary;
std::map< std::string, std::string >	_contentDisposition;
//std::string	

void		_parseContentLength( std::string request )
{
	if ( !request.compare( 0, 16, "Content-Length: ") )
		_contentLength = std::strtoul( request.substr( 16, ( request.find( "\0", 0 ) - 16 ) ).c_str() , NULL, 0 );
}

void		_parseContentType( std::string request )
{
	if ( !request.compare( 0, 14, "Content-Type: ") && request.find( "boundary=", 0 ) != std::string::npos )
	{
		_contentType = request.substr( 14, ( request.find( ";", 0 ) - 14 ) );
		_boundary = request.substr( request.find_last_of( "-" ) + 1,
			request.find( "\0", 0 ) - request.find_last_of( "-" ) + 1 );
	}
	else if ( !request.compare( 0, 14, "Content-Type: ") )
		_contentType = request.substr( 14, ( request.find( "\0", 0 ) - 14 ) );
}

void		_parseContentDisposition( std::string request )
{
	if ( !request.compare( 0, 21, "Content-Disposition: ") )
	{
		_contentDisposition["content-type"] = request.substr( 21, ( request.find( ";", 0 ) - 21 ) );

		_contentDisposition["name"] = request.substr( request.find( "=", 0 ) + 2,
			( (request.find_last_of( ";" ) - 1) - (request.find( "=", 0 ) + 2 )) );

		_contentDisposition["filename"] = request.substr( request.find_last_of( "=" ) + 2,
			 ( request.find_last_of( "\"" ) ) - ( request.find_last_of( "=" ) + 2  ) );
	}
}


void		parseRequest(std::string request)
{
	std::string			line;
	std::stringstream 	ss;
	size_t              pos;
	ss << request;

	while ((pos = request.find("\r")) != std::string::npos)
	  	request.erase(pos, 1);

	std::cout << "*********IN FUNCTIN**********\n\n";
	while (!ss.eof())
	{
		std::getline(ss, line);
		_parseMethodAndPath(line);
		_parseHost(line);
		_parseAccept(line);
		_parseContentLength( line );
		_parseContentType( line );
		_parseContentDisposition( line );
		std::cout << line << "\n";
	}
	std::cout << "\n\n*********VARIABLES**********\n\n";

		std::cout << "method = " << _method << "\n";
		std::cout << "_addr = " << _addr << "\n";
		std::cout << "port = " << _port << "\n";
		std::cout << "_path = " << _path << "\n";
		std::cout << "_contentLength = " << _contentLength << "\n";
		std::cout << "_contentType = " << _contentType << "\n";
		std::cout << "_boundary = " << _boundary << "\n";
		std::cout << "_contentDisposition = " << "\n";
		for( std::map< std::string, std::string >::iterator it = _contentDisposition.begin();
				it != _contentDisposition.end(); it++ )
			std::cout << "	" << it->first << " = " << it->second << std::endl;

		std::cout << "accept options :\n";
		std::cout << "<<\n";
		for (std::vector<std::string>::iterator it = _accept.begin(); it != _accept.end(); it++){
			std::cout << *it << "\n";
		}
		std::cout << ">>" << "\n";

}

int main()
{
	std::string request = "GET /index.html HTTP/1.1\nHost: localhost:8080\nConnection: keep-alive\nCache-Control: max-age=0\nsec-ch-ua: \"Google Chrome\";v=\"107\", \"Chromium\";v=\"107\", \"Not=A?Brand\";v=\"24\"\nsec-ch-ua-mobile: ?0\nsec-ch-ua-platform: \"Linux\"\nUpgrade-Insecure-Requests: 1\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/107.0.0.0 Safari/537.36\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\nSec-Fetch-Site: none\nSec-Fetch-Mode: navigate\nSec-Fetch-User: ?1\nSec-Fetch-Dest: document\nAccept-Encoding: gzip, deflate, br\nAccept-Language: fr-FR,fr;q=0.9,en-US;q=0.8,en;q=0.7";

	std::string	postRequest1 = "Host: localhost:8080\nUser-Agent: curl/7.81.0\nAccept: */*\nContent-Length: 40\nContent-Type: application/x-www-form-urlencoded\nname=linuxize&email=linuxize@example.com;";
	std::string	postRequestFile = "POST / HTTP/1.1\nHost: 127.0.0.1:8080\nUser-Agent: curl/7.81.0\nAccept: */*\nContent-Length: 6712\nContent-Type: multipart/form-data; boundary=------------------------dfea8bdf67e372e1";
	std::string	postRequestFile2 = "--------------------------dfea8bdf67e372e1\nContent-Disposition: form-data; name=\"file1\"; filename=\"Makefile\"\nContent-Type: application/octet-stream\ncontent-file";
    parseRequest(postRequestFile2);
    return (0);

}
