#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>


/*=====================*/
#include <unistd.h>
#include <fcntl.h>
#include <memory>

# define NB_GCI_ENV_VAR 17

char**	ft_split(char const *s, char c);

void	_initVar(std::string *var){

	var[0] = "1986";
	var[1] = "text/html";
	var[2] = "CGI/1.1";
	var[3] = "/process/";
	var[4] = std::string("srcs/") + std::string("process/");
	var[5] = "Name=Sacha&Name2=Amir&Name3=Maxence";//req.querySting;
	var[6] = "NULL";
	var[7] = "NULL";
	var[8] =  "NULL"; // client_login;
	var[9] = "NULL"; //user_login;
	var[10] = "GET";
	var[11]= "/class/";
	var[12] = "NULL";
	var[13] = "NULL";
	var[14] = "HTTP/1.1";
	var[15] = "SAMserver/1.0";
	var[16] = "COOKIE";//req.cookieString;
}

char**	buildCGIenv(void)
{
	int i = 0;
	size_t pos = 0;
	std::string	starter = "CONTENT_LENGTH= CONTENT_TYPE= GATEWAY_INTERFACE= PATH_INFO= PATH_TRANSLATED= QUERY_STRING= REMOTE_ADDR= REMOTE_HOST= REMOTE_IDENT= REMOTE_USER= REQUEST_METHOD= SCRIPT_NAME= SERVER_NAME= SERVER_PORT= SERVER_PROTOCOL= SERVER_SOFTWARE= HTTP_COOKIE= ";
	std::string	var[NB_GCI_ENV_VAR];

	_initVar(var);
	while ((pos = starter.find(" ", pos)) != std::string::npos){
		starter.insert(pos, var[i]);
		pos += var[i].size() + 1;
		i++;
	}
	return (ft_split(starter.c_str(), ' '));
}


