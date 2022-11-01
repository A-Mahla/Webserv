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
std::string _port, _addr;
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


void		_parseMethod(std::string request)
{
	size_t pos = 0;

	if ((pos = request.find("GET")) != std::string::npos)
		_method = GET;
	else if ((pos = request.find("POST")) != std::string::npos)
		_method = POST;
	else if ((pos = request.find("DELETE")) != std::string::npos)
		_method = DELETE;
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
        _parseMethod(line);
        _parseHost(line);
        _parseAccept(line);
        std::cout << line << "\n";
    }
    std::cout << "\n\n*********VARIABLES**********\n\n";
    
        std::cout << "method = " << _method << "\n";
        std::cout << "_addr = " << _addr << "\n";
        std::cout << "port = " << _port << "\n";

         std::cout << "[ ";
        for (std::vector<std::string>::iterator it = _accept.begin(); it != _accept.end(); it++){
            std::cout << *it << "\n";
        }
        std::cout << " ]" << "\n";
	// size_t pos = 0;

	

	// if (!_parseMethod(request))
	// 	return ;
	
	// if ((pos = request.find("HTTP/1.1")) != std::string::npos)
	// {
	// 	request.erase(0, 1);
	// 	_path = string(request.begin(), request.begin() + pos - 2);
	// 	pos =  request.find("\n");
	//  	request.erase(0, pos + 1);
	// }
	// else
	// 	return ;
		
		
	
		
	// request.push_back(';');

	// if (!_parseHost(request))
	// 	return ;
		
	/*===========================================================
	std::cout << RED << "request: " << SET << "method: " << _method <<  "\n";
	std::cout << "port: " << _port << "\n" << "addr: " <<  _addr << "\n" ;
	std::cout << "path: " << _path  <<  std::endl;*/
	
}


int main()
{
    std::string request = "GET /index.html HTTP/1.1\nHost: localhost:8080\nConnection: keep-alive\nCache-Control: max-age=0\nsec-ch-ua: \"Google Chrome\";v=\"107\", \"Chromium\";v=\"107\", \"Not=A?Brand\";v=\"24\"\nsec-ch-ua-mobile: ?0\nsec-ch-ua-platform: \"Linux\"\nUpgrade-Insecure-Requests: 1\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/107.0.0.0 Safari/537.36\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\nSec-Fetch-Site: none\nSec-Fetch-Mode: navigate\nSec-Fetch-User: ?1\nSec-Fetch-Dest: document\nAccept-Encoding: gzip, deflate, br\nAccept-Language: fr-FR,fr;q=0.9,en-US;q=0.8,en;q=0.7";

	std::string	postRequest1 = "Host: localhost:8080\nUser-Agent: curl/7.81.0\nAccept: */*\nContent-Length: 40\nContent-Type: application/x-www-form-urlencoded\nname=linuxize&email=linuxize@example.com;"
    parseRequest(request);
    return (0);

}
