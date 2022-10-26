/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxenceeudier <maxenceeudier@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 09:37:12 by maxenceeudi       #+#    #+#             */
/*   Updated: 2022/10/26 12:26:04 by maxenceeudi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/webserv.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <utility>

using namespace std;

/*========================*/
/*                        */
/*        TOOLS           */
/*                        */
/*========================*/

bool    is_white_space(char val)
{
    char delimiter[] = {' ', '\t', '\n', '\r', '\v', '\f'};
    size_t i = 0;
    while (i < (sizeof(delimiter) / sizeof(char)))
    {
        if (val == delimiter[i])
            return (true);
        i++;
    }
    return (false);
}

int     suppr_white_space(string &line)
{
    int pos = 0;
    int size = line.size();

    while ((pos < size) && is_white_space(line[0]))
    {
        line.erase(0, 1);
        pos++;
    }
    return (pos);
}

bool    check_end_line(string &line)
{
    for (string::const_iterator ite = line.end() - 1; ite != line.begin(); ite--)
    {
        if (!is_white_space(*ite))
        {
            if (*ite == ';')
            {
                line.erase(ite, line.end());
                return (true);
            }   
            return (false);
        }
    }
    return (false);
}

string  parse(const string &line, string comp)
{
    string cpy = line;
    size_t  found;
     
    found = cpy.find(comp);
    if (found)
        return ("error");
    cpy.erase(0, comp.size());

    suppr_white_space(cpy);
    
    if (cpy[0] == ':' || cpy[0] == '=')
        cpy.erase(0, 1);

    suppr_white_space(cpy);
    
    return (cpy);
}

bool    get_the_info_i_need(string &line, string atribut, string &rslt)
{
    if (!check_end_line(line))
        return (false);
    rslt = parse(line, atribut);
    if (rslt == "error")
        return (false);
    return (true);
}

size_t     get_nb_of_file(string line)
{
    size_t count = 0;
    size_t i = 0;
    while (i < line.size())
    {
        if (!is_white_space(line[i]))
        {
            count++;
            while (i < line.size() && !is_white_space(line[i]))
                i++;
        }
        else
            i++;
    }
    return (count);
}

bool    get_key_map_error(string &rslt, map<string, string> &error_map)
{
    string  temp;
    pair<string, string>    tmp;
    int i = 0;
    int len;

    while (rslt[i] && (rslt[i] != '/'))
    {
        len = 0;
        while (isnumber(rslt[++len]));
        if (len % 3)
            return (false);

        temp.insert(temp.begin(), rslt.begin(), rslt.begin() + 3);
        tmp = pair<string, string>(temp, "");
        error_map.insert(tmp);
        temp.clear();
        rslt.erase(0, 3);
        suppr_white_space(rslt);
    }
    return (true);
}

bool    get_value_map_error(string &rslt, map<string, string> &error_map)
{
    map<string, string>::iterator it;
    string temp;
    size_t i;
    size_t nb_of_file;
    if (rslt[0] == '/')
        rslt.erase(0, 1);
    else
        return (false);
        
    suppr_white_space(rslt);
    
    it = error_map.begin();
    temp = rslt;
    nb_of_file = get_nb_of_file(rslt);
    if (nb_of_file > 1)
    {
        if (error_map.size() != nb_of_file )
            return (false);
        while (it != error_map.end())
        {
            i = 0;
            while(i < rslt.size() && !is_white_space(rslt[i]))
                i++;
            it->second.insert(it->second.begin(), rslt.begin(), rslt.begin() + i);
            rslt.erase(0, i);
            suppr_white_space(rslt);
            it++;
        }
    }
    else
    {
        while (it != error_map.end())
        {
            i = 3;
            rslt = temp;
            while (i--)
            {
                if (rslt[i] == 'x')
                    rslt[i] = it->first[i]; 
            }
            it->second = rslt;
            it++;
        }
    }
    return (true);
}

/*==============================*/
/*                              */
/*        CLIENT BODY           */
/*                              */
/*==============================*/

bool    client_body(string &line, Server &server)
{
    string rslt;
    int     i = 0;

    if (!get_the_info_i_need(line, "client_body_buffer_size", rslt))
        return (false);
        
    while (isnumber(rslt[++i]));

    if (rslt[i] && (tolower(rslt[i]) == 'k') && !rslt[i + 1])
    {
        rslt.erase(i, 1);
        rslt += "000";
    }
    else if (rslt[i])
        return (false);
    
    if (server.get_is_set("clientbody"))
        return (false);
        
    server.set_clientBody(atoi(rslt.c_str()));
    server.set_is_set("clientbody");

    //default value depend of architecture: 8K if 32 bits / 16k if 64 bits
    return (true);
    
}


/*==============================*/
/*                              */
/*        ERROR_PAGE            */
/*                              */
/*==============================*/

bool    error_page(string &line, Server &server)
{
    string rslt;
    map<string, string> error_map;

    if (!get_the_info_i_need(line, "error_page", rslt))
        return (false);
    
    if (!get_key_map_error(rslt, error_map))
        return (false);

    if (!get_value_map_error(rslt, error_map))
        return (false);
            
    map<string, string> &map_error = server.get_error_pages();
    map_error.insert(error_map.begin(), error_map.end());
    server.set_is_set("error_page");
    
    //default value: none
    return (true);
}


/*==============================*/
/*                              */
/*            INDEX             */
/*                              */
/*==============================*/

bool    index(string &line, Server &server)
{
    string rslt;
    int len = 0;
    string tmp;
    vector<string>  temp;
    string::iterator it;
    
    if (!get_the_info_i_need(line, "index", rslt))
        return (false);
        
    it = rslt.begin();
    while (it != rslt.end())
    {
        if (is_white_space(*it))
        {
            while ((it != rslt.end()) && is_white_space(*it))
                it++;
        }
        else
        {
            len = 0;
            while ((it + len != rslt.end()) && !is_white_space(*(it + len)))
                len++;
            tmp.insert(tmp.begin(), it, it + len);
            if (tmp != "index.html")
                temp.push_back(tmp);
            tmp.clear();
            it += len;
        }
    }
    if (server.get_is_set("index"))
        return (false);

    vector<string> &index_vector = server.get_index();
    index_vector.insert(index_vector.end(), temp.begin(), temp.end());

    server.set_is_set("index");
    
    //default value: index.html
    return (true);
}


/*==============================*/
/*                              */
/*             ROOT             */
/*                              */
/*==============================*/

bool    root(string &line, Server &server)
{
    string  rslt;

    if (!get_the_info_i_need(line, "root", rslt))
        return (false);
    
    if (server.get_is_set("root") || get_nb_of_file(rslt) != 1)
        return (false);
        
    server.set_is_set("root");
    server.set_root(rslt);
    
    return (true);
}

//to comple the main : 
//c++ -Wall -Wextra -Werror -std=c++98 ./srcs/max_methode/parse_utils.cpp ./srcs/class/Server.cpp
int main ()
{
    /*============================*/
    /*   client body              */
    /*============================*/
    Server  server;
    string line1 = "client_body_buffer_size : 8k;    ";
    string line2 = "client__buffer_size : 16k;      ";
    client_body(line1, server);
    client_body(line2, server);

    cout << GREEN <<  "clientbody:\n" << SET << server.get_clientBody() <<  endl;
    cout << endl;


    /*============================*/
    /*   error pages              */
    /*============================*/
    string  error_page1 = "error_page 404             /404.html;";
    string  error_page2 = "error_page 500 502 503 504           / 50x.html ;  ";
    string  error_page3 = "error_page 200 201 202 203       / 200.html 201.html 202.html 203.html; ";
    error_page(error_page1, server);
    error_page(error_page2, server);
    error_page(error_page3, server);

    map<string, string>::iterator it = server.get_error_pages().begin();
    cout << GREEN <<  "error_page:\n" << SET;
    while (it != server.get_error_pages().end())
    {
        cout << "key: " <<  it->first << " / value: " << it->second << endl;
        it++;
    }
    cout << endl;

    /*============================*/
    /*   index                    */
    /*============================*/
    string index1 = "inde index.html index.php test.html;";
    string index2 = "index test1.html index2.php test.html index.html       ;   ";
    index(index1, server);
    index(index2, server);

    cout << GREEN << "index:\n" << SET;
    cout << "[ ";
    for (vector<string>::iterator it = server.get_index().begin(); it != server.get_index().end() - 1; it++)
        cout << *it << ", ";
    cout << server.get_index().back() <<  " ]" << endl;
    cout << endl;
    

    /*============================*/
    /*   root                     */
    /*============================*/
    string root1 = "root         /srcs/ ywb hjq;";
    string root2 = "root     ./srcs;";

    root(root1, server);
    root(root2, server);

    cout << GREEN << "root:\n" << SET << server.get_root() << endl;

    
    return (0);
}