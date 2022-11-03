#!/bin/bash

c++ ./cgi/test.cpp -o cgi/test.cgi;
c++ -Wall -Wextra -Werror -std=c++98 ./cgi/cgi.cpp  -o cgi/cgi;

./cgi/cgi;