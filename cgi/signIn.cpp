/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signIn.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxenceeudier <maxenceeudier@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 09:33:17 by meudier           #+#    #+#             */
/*   Updated: 2022/11/04 16:06:45 by maxenceeudi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include <iostream>
# include <string>
# include <sstream>
#include <fstream>

std::string readFile(std::string path)
{
	std::ifstream 	ifs;
	std::string		file_content;
	std::string		filename = path;
	int				length;
	char			*buff;

	ifs.open(filename.c_str(), std::ifstream::in);

	if ( !ifs.is_open() )
		return ("");

	ifs.seekg (0, ifs.end);
	length = ifs.tellg();
	ifs.seekg (0, ifs.beg);

	buff = new char[length + 1];
	ifs.read(buff, length);
	if (ifs.fail())
	{
        ifs.close();
        return ("");
    }
	ifs.close();
	buff[length] = '\0';
	file_content = buff;
	delete [] buff;

	return (file_content);
}

std::string getName(std::string qs)
{
    size_t  pos = 0;

    pos = qs.find("=", 0);
    qs.erase(0, pos + 1);
    pos = qs.find("&", 0);
    qs.erase(pos, qs.size() - pos);
    return (qs);
}

int main()
{
    //char *qs = getenv("QUERY_STRING");
    //if (!qs)
    //    return (0);
    std::string queryString = getenv("QUERY_STRING");
    std::string name = getName(queryString);

    std::string content_body = readFile("./html/home.html");
    std::string insertNameHere = "Welcome in our webserv ";
    std::string line;
    size_t      pos = 0;
    size_t      pos_temp = 0;
    size_t      insert_pos = 0;
    std::stringstream ss;
    ss << content_body.size() + name.size();

    /*==========================*/
    /*    make header whith env */
    /*==========================*/

    std::cout << "HTTP/1.1 200 OK\n";
    std::cout << "Content-Type: text/html\r\n";
    std::cout << "Content-Length: ";
    std::cout <<  ss.str();
    std::cout << "\n\n";

    /*============================*/
    /*             body           */
    /*============================*/
    while((pos = content_body.find("\n", pos_temp)) != std::string::npos)
    {
        pos++;
        line = content_body.substr(pos_temp, pos - pos_temp);
        if ((insert_pos = line.find(insertNameHere, 0)) != std::string::npos)
        {
            content_body.insert(pos_temp + insert_pos + insertNameHere.size(), name);
            pos += name.size();
        }
        std::cout << content_body.substr(pos_temp, pos - pos_temp);
        pos_temp = pos;
    }
    std::cout << content_body.substr(pos_temp, pos - pos_temp);

    /*============================*/
    /*             footer          */
    /*============================*/
    std::cout << "\r\n\r\n";

    return (0);
}
