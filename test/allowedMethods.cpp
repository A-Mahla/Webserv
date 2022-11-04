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

bool    checkMethodes(std::string str, Server & serv)
{
    if ((str.substr(0, 3)).find("GET", 0) != std::string::npos && !serv.getAllowGet()){
        if (str[3] == ':')
            if (checkMethodes(str.c_str() + 4, serv)){
                serv.setAllowGet(true);
                return (true);
            }
        if (str[3] == ';'){
            serv.setAllowGet(true);
            return (true);
        }
    }
    else if ((str.substr(0, 4)).find("POST", 0) != std::string::npos && !serv.getAllowPost()){      
        if (str[4] == ':'){
            if (checkMethodes(str.c_str() + 5, serv)){
                serv.setAllowPost(true);
                return (true);
            }
        }
        if (str[4] == ';') {
            std::cout << "je suis sense passer par la \n";
            serv.setAllowPost(true);
            return (true);
        }
    }
    else if ((str.substr(0, 6)).find("DELETE", 0) != std::string::npos && !serv.getAllowDelete()){
        if (str[6] == ':'){
            if (checkMethodes(str.c_str() + 7, serv)){
                serv.setAllowDelete(true);
                return (true);
            }
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
    if ( !(str.compare(0, 9, "methodes ")) ){
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


    std::string test = "methodes         GET:GET:DELETE;";
    Server serv;

    int ret = allowedMethodsParse(test, serv);

    std::cout << "return is : " << ret << "\n";
    std::cout << "printing the values I got : " << "\n"; 
    std::cout << "GET :" << serv.getAllowGet() << std::endl;
    std::cout << "POST :" << serv.getAllowPost() << std::endl;
    std::cout << "DELETE :" << serv.getAllowDelete() << std::endl;
    return (0);
}