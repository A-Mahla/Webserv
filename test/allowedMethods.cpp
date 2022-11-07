#include <string>
#include <iostream>
#include <cstdlib>
#include <ctype.h>
#include <locale>


class Server
{
	private:
		bool								_allowGet;
		bool								_allowPost;
		bool								_allowDelete;
	public:
        Server() : _allowGet(0), _allowPost(0), _allowDelete(0) 
        {return ;}
        bool    getAllowGet(){return (_allowGet);}
        bool    getAllowPost(){return (_allowPost);}
        bool    getAllowDelete(){return (_allowDelete);}
        void    setAllowDelete(bool val){_allowDelete = val;}
        void    setAllowPost(bool val){_allowPost = val;}
        void    setAllowGet(bool val){_allowGet = val;}
};

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

bool    checkMethodes(std::string str, Server & serv)
{
    std::cout << "INPUT IS : " << str << std::endl;
    if ((str.substr(0, 3)).find("GET", 0) != std::string::npos && !serv.getAllowGet()){
        if (str[3] == ':') {
            serv.setAllowGet(true);
            return (checkMethodes(str.c_str() + 4, serv));
        }
        if (str[3] == ';'){
            serv.setAllowGet(true);
            return (true);
        }
    }
    else if ((str.substr(0, 4)).find("POST", 0) != std::string::npos && !serv.getAllowPost()){      
        if (str[4] == ':'){
            serv.setAllowPost(true);
            return (checkMethodes(str.c_str() + 5, serv));
        }
        if (str[4] == ';') {
            serv.setAllowPost(true);
            return (true);
        }
    }
    else if ((str.substr(0, 6)).find("DELETE", 0) != std::string::npos && !serv.getAllowDelete()){
        if (str[6] == ':'){
            serv.setAllowDelete(true);
            return (checkMethodes(str.c_str() + 7, serv));
        }
        if (str[6] == ';'){
            serv.setAllowDelete(true);
            return (true);
        }
    }
    return (false);
}


bool	allowedMethodsParse(std::string str, Server & serv)
{
    int i = 0;
    if ( !(str.compare(0, 9, "methodes ")) && checkOccurance(str, ";") == 1 && afterSemiColon(str)){
        for (i = 9; str[i] == ' ' || str[i] == '\t'; i++);
        if (checkMethodes(str.c_str() + i, serv))
            return (true);
    }
    serv.setAllowGet(false);
    serv.setAllowDelete(false);
    serv.setAllowPost(false);
    return (false);
}

int main(void){


    std::string test = "methodes                POST;";
    Server serv;


    int ret = allowedMethodsParse(test, serv);

    std::cout << "return is : " << ret << "\n";
    std::cout << "printing the values I got : " << "\n"; 
    std::cout << "GET :" << serv.getAllowGet() << std::endl;
    std::cout << "POST :" << serv.getAllowPost() << std::endl;
    std::cout << "DELETE :" << serv.getAllowDelete() << std::endl;
    return (0);
}