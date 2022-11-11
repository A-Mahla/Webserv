#!/bin/bash

c++ -Wall -Wextra -Werror -std=c++98 ./cgi/signIn.cpp -o cgi/signIn.cgi;
c++ -Wall -Wextra -Werror  -std=c++98 ./cgi/autoindex.cpp -o cgi/autoindex.cgi;
