#include <iostream>
#include <string>
#include <cstdlib>

#define NB_ELEMENTS 17


char	**ft_split(char const *s, char c);

enum e_method
{
    BAD_REQUEST,
    GET,
    POST,
    DELETE
};

std::string content_lenght = "1256";
std::string content_type = "application/url_encoded";
e_method					_method = POST;
std::string					_path = "/index.html";
std::string					_root = "/bin/";
std::string					_port = "8080";
std::string					_addr = "localhost";


void	initVar(std::string *var /*, Request const & req, Server const & serv*/){
	var[0] = content_lenght; //req.content_lenght;
	var[1] = content_type; //req.content_type;
	var[2] = "CGI/1.1";
	var[3] = _path;//req.getPath();
	var[4] = "/bin" + _path;//serv.get_root() + req.getPath();
	var[5] = "queryString";//req.querySting;
	var[6] = "originString";//req.origin;
	var[7] = "NULL";
	var[8] =  "NULL"; // client_login;
	var[9] = "NULL"; //user_login;
	if (_method == GET)
		var[10] = "GET";
	else if (_method == POST)
		var[10] = "POST";
	else if (_method == DELETE)
		var[10] = "DELETE";
	var[11]= "test.cgi";//req.scriptName;
	var[12] = "ServerName";//serv.getServerName();
	var[13] = "Port";//serv.getPort();
	var[14] = "HTTP/1.1";
	var[15] = "SAMserver/1.0";
	var[16] = "COOKIE";//req.cookieString;
}

char**	buildCGIenv(/*Request & req, Server & serv*/void)
{
	int i = 0;
	size_t pos = 0;
	std::string	starter = "CONTENT_LENGTH= CONTENT_TYPE= GATEWAY_INTERFACE= PATH_INFO= PATH_TRANSLATED= QUERY_STRING= REMOTE_ADDR= REMOTE_HOST= REMOTE_IDENT= REMOTE_USER= REQUEST_METHOD= SCRIPT_NAME= SERVER_NAME= SERVER_PORT= SERVER_PROTOCOL= SERVER_SOFTWARE= HTTP_COOKIE= ";
	std::string	var[NB_ELEMENTS];

	initVar(var/*, req, serv*/);
	while ((pos = starter.find(" ", pos)) != std::string::npos){
		starter.insert(pos, var[i]);
		pos += var[i].size() + 1;
		i++;
	}
	std::cout << "\n\n************PRINTING TEST*************\n\n";
	std::cout << starter << std::endl;
	//. . . the function take information from server and request to insert into
	//       starter string.
	// when everything is set, we split and return the char**

	return (ft_split(starter.c_str(), ' '));
}

int main(void){

	char **test = buildCGIenv();

	std::cout << "\n\n----------------FINAL_TEST-----------\n\n";

	for (int i = 0; *(test + i); i++){
		std::cout << *(test + i) << std::endl;
	}

	for (int i = 0; *(test + i); i++){
		free(*(test + i));
	}

	free(test);
	return (0);
}
