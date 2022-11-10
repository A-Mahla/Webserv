#include <iostream>
#include <string>

class Server
{

	private:
		bool		_redirect;
		std::string	_redirectStr;
	public:

		Server( void ) : _redirect(false) {}
		~Server( void ) {}

		bool				getRedirect( void ){ return (_redirect); }
		std::string const &	getRedirectStr( void ){ return (_redirectStr); }
		void	setRedirect( bool redirect ){ _redirect = redirect; }
		void	setRedirectStr( std::string redirectStr ){ _redirectStr = redirectStr; }
};

int	checkOccurance(std::string & str, const char * toFind)
{
	int	i = 0;
	size_t occurs = 0;

	while (((occurs = str.find(toFind, occurs))) != std::string::npos){
		occurs++;
		i++;
	}
	return (i);
}

bool    afterSemiColon(std::string & str)
{
    int i = 0;
    for (std::string::iterator it = str.end() - 1; *it != ';' && it != str.begin(); it--, i++){
        if (*it != '\t' && *it != ' ') {
            if (*it == '\0')
                std::cout << "ici le test [" << *it << "] " << i << "\n";
            return (false);
        }
    }
    return (true);
}

bool	redirectParse( const std::string str_const, Server & serv )
{
	int i = 0;
	std::string	str = str_const;
	if ((str.compare(0, 9, "redirect ") == 0 || str.compare(0, 9, "redirect\t") == 0) && checkOccurance(str, ";") == 1 && afterSemiColon(str))
	{
		for (i = 8; str[i] == ' ' || str[i] == '\t'; i++){ /* just skipping space and tab */}
		serv.setRedirectStr( str.substr(i, str.find(";", 0) - i) );
		serv.setRedirect(true);
		std::cout << "ici le test : [" << serv.getRedirectStr() << "]\nredirect bool is : " << serv.getRedirect() << "\n";
		return (true);
	}
	else
		return (false);
}

int main(){

	Server serv;
	redirectParse("redirect jesuiuntest;", serv);
	std::cout << "IN THE MAIN :\n\n";
	std::cout << "ici le test : [" << serv.getRedirectStr() << "]\nredirect bool is : " << serv.getRedirect() << "\n";
	return (0);
}
