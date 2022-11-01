#include <iostream>
#include <string>

char**	buildEnvVar(Request & req, Server & serv)
{
	std::string	starter = "CONTENT_LENGTH= CONTENT_TYPE= GATEWAY_INTERFACE= PATH_INFO= PATH_TRANSLATED= QUERY_STRING= REMOTE_ADDR= REMOTE_HOST= REMOTE_IDENT= REMOTE_USER= REQUEST_METHOD= SCRIPT_NAME= SERVER_NAME= SERVER_PORT= SERVER_PROTOCOL= SERVER_SOFTWARE= HTTP_COOKIE= ";
	//. . . the function take information from server and request to insert into
	//       starter string.
	// when everything is set, we split and return the char**
}

void	execCGI(char **env){
	// . . . do the fork and everything to execute the CGI
}

int main(void){
	std::string request = "\nPOST /index.html HTTP/1.1\nHost: localhost:8080\nConnection: keep-alive\nContent-Length: 17\nCache-Control: max-age=0\nsec-ch-ua: \"Chromium\";v=\"106\", \"Google Chrome\";v=\"106", \"Not;A=Brand\";v=\"99\"\nsec-ch-ua-mobile: ?0\nsec-ch-ua-platform: \"Linux\"\nUpgrade-Insecure-Requests: 1\nOrigin: http://localhost:8080\nContent-Type: application/x-www-form-urlencoded\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/106.0.0.0 Safari/537.36\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\nSec-Fetch-Site: same-origin\nSec-Fetch-Mode: navigate\nSec-Fetch-User: ?1\nSec-Fetch-Dest: document\nReferer: http://localhost:8080/index.html\nAccept-Encoding: gzip, deflate, br\nAccept-Language: en-US,en;q=0.9"
	// . . . test

	return (0);
}
