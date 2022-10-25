/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxenceeudier <maxenceeudier@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 09:37:12 by maxenceeudi       #+#    #+#             */
/*   Updated: 2022/10/25 18:45:47 by maxenceeudi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/webserv.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <utility>

using namespace std;


bool is_delimiter(char val)
{
    char delimiter[] = {' ', '\t', '\n', '\r', '\v', '\f'};
    int i = 0;
    while (i < 6)
    {
        if (val == delimiter[i])
            return (true);
        i++;
    }
    return (false);
}

bool check_end_line(string &line)
{
    for (string::const_iterator ite = line.end() - 1; ite != line.begin(); ite--)
    {
        if (!is_delimiter(*ite))
        {
            if (*ite == ';')
            {
                line.erase(ite);
                return (true);
            }
                
            return (false);
        }
    }
    return (false);
}

string parse(const string &line, string comp)
{
    string cpy = line;
    size_t  pos = 0;
    int     i = 0;
    string delimiter[] = {" ", "\t", "\n", "\r", "\v", "\f"};
    
    while (i < delimiter->size())
    {
        while ((pos = cpy.find(delimiter[i])) < cpy.size())
            cpy.erase(pos, delimiter[i].size());
        i++;
    }
    
    size_t  found = cpy.find(comp);
    if (found)
        return ("error");
    cpy.erase(0, comp.size());
    if (cpy[0] == ':' || cpy[0] == '=')
        cpy.erase(0, 1);
    return (cpy);
}

string parse2(const string &line, string comp)
{
    string cpy = line;
    size_t  found = cpy.find(comp);

    if (found)
        return ("error");
    cpy.erase(0, comp.size());
    if (cpy[0] == ':' || cpy[0] == '=')
        cpy.erase(0, 1);
    return (cpy);
}


bool    clien_body(string &line, Server &server)
{
    int     i = 0;

    if (!check_end_line(line))
        return (false);
    string rslt = parse(line, "client_body_buffer_size");
    if (rslt == "error")
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

bool    error_page(string &line, Server &server)
{
    if (!check_end_line(line))
        return (false);
    string  rslt = parse(line, "error_page");
    map<string, string> error_map;
    string  temp;
    pair<string, string>    tmp;
    int i = 0;
    int len = 0;

    if (rslt == "error")
        return (false);

    while (isnumber(rslt[++len]));

    if (len % 3)
        return (false);

    while (i < len )
    {
        temp.insert(temp.begin(), rslt.begin(), rslt.begin() + 3);
        tmp = pair<string, string>(temp, "");
        error_map.insert(tmp);
        temp.clear();
        rslt.erase(0, 3);
        i += 3;
    }

    if (rslt[0] == '/')
        rslt.erase(0, 1);
    else
        return (false);

    map<string, string>::iterator it = error_map.begin();
    temp = rslt;
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
    
    map<string, string> &map_error = server.get_error_pages();
    map_error.insert(error_map.begin(), error_map.end());
    server.set_is_set("error_page");
    
    //default value: none
    return (true);
}

bool    index(string &line, Server &server)
{
    if (!check_end_line(line))
        return (false);
    string delimiter[] = {" ", "\t", "\n", "\r", "\v", "\f"};
    string rslt = parse2(line, "index");
    int len = 0;
    vector<string>  temp;
    string tmp;

    if (rslt == "error")
        return (false);

    string::iterator it = rslt.begin();
    while (it != rslt.end())
    {
        if (is_delimiter(*it))
        {
            while ((it != rslt.end()) && is_delimiter(*it))
                it++;
        }
        else
        {
            len = 0;
            while ((it + len != rslt.end()) && !is_delimiter(*(it + len)))
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

int main ()
{
    /*============================*/
    /*   client body              */
    /*============================*/
    Server  server;
    string line1 = "client_body_buffer_size : 8k;    ";
    string line2 = "client__buffer_size : 16k;      ";
    clien_body(line1, server);
    clien_body(line2, server);

    cout << GREEN <<  "clientbody: " << SET << server.get_clientBody() <<  endl;
    cout << endl;


    /*============================*/
    /*   error pages              */
    /*============================*/
    string  error_page1 = "error_page 404             /404.html;";
    string  error_page2 = "error_page 500 502 503 504  /50x.html  ; ";
    string  error_page3 = "error_page 500    502 503 504  /  50x.html          ";
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
    
    return (0);
}