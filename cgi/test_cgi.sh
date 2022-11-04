#!/bin/bash

c++ -Wall -Wextra -Werror ./cgi/signIn.cpp -o cgi/signIn.cgi;
c++ -Wall -Wextra -Werror -g ./cgi/autoindex.cpp -o cgi/autoindex.cgi;
