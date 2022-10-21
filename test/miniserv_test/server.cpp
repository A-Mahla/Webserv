/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxenceeudier <maxenceeudier@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 10:44:53 by maxenceeudi       #+#    #+#             */
/*   Updated: 2022/10/21 11:57:16 by maxenceeudi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sys/types.h>

#include <vector>
#include <algorithm>

# define GREEN "\033[0;32m"
# define RED "\033[0;31m"
# define YELLOW "\033[0;33m"
# define SET "\033[0m"

using namespace std;

//int create_socket(int port)
//{
//    int                 listen_fd;
//    struct sockaddr_in  address;

    /*============================*/
    /*     create socket          */
    /*============================*/
//    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0) ) < 0)
//    {
//        perror("Fail: socket() error\n");
//        exit(EXIT_FAILURE);
//    }

    /*================================*/
    /* allowed to reuse addr and port */
    /*================================*/
//    int on = 1;
//    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int)) < 0)
//    {
//        perror("Fail: setsockopt() error\n");
//        close (listen_fd);
//        exit(EXIT_FAILURE);
//    }
//    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(int)) < 0)
//    {
//        perror("Fail: setsockopt() error\n");
//        close (listen_fd);
//        exit(EXIT_FAILURE);
//    }
    
    /*============================*/
    /*    make non-bloking fd     */
    /*============================*/
//    if (fcntl(listen_fd, F_SETFL, O_NONBLOCK) < 0)
//    {
//        perror("Fail: fcntl() error\n");
//        close (listen_fd);
//        exit(EXIT_FAILURE);
//    }

    /*============================*/
    /*       build address        */
    /*============================*/
//    memset(reinterpret_cast<char *>(&address), 0, sizeof(address)); 
//    address.sin_family = AF_INET;
//    address.sin_addr.s_addr = htonl(INADDR_ANY);
//    address.sin_len = htons(port);

    /*============================*/
    /*     bind to an address      */
    /*============================*/
//    if (bind(listen_fd, reinterpret_cast<sockaddr *>(&address), sizeof(address)) < 0)
//    {
//        perror("Fail: bind() error\n");
//        close (listen_fd);
//        exit(EXIT_FAILURE);
//    }

    /*============================*/
    /*          listen            */
    /*============================*/
//    if ( listen(listen_fd, 100) < 0)
//    {
//        perror("Fail: listen() error\n");
//        close (listen_fd);
//        exit(EXIT_FAILURE);
//    }
//    cout << "A new sockket is listening on port : " << port << endl;
//    return (listen_fd);
//}


int create_socket(int port)
{
	int listen_fd;
	struct sockaddr_in servaddr;

	/**========================================================================
	 * ?                         CREATE SOCKET
	 *========================================================================**/
		if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			std::cerr << "error: socket()" << std::endl;
			exit(1);
		}

        int on = 1;
        if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(int)) < 0)
        {
            perror("Fail: setsockopt() error\n");
            close (listen_fd);
            exit(EXIT_FAILURE);
        }
        if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int)) < 0)
        {
            perror("Fail: setsockopt() error\n");
            close (listen_fd);
            exit(EXIT_FAILURE);
        }

	/**========================================================================
	 * ?                         TURN SOCKET INTO NON BLOCKING
	 *========================================================================**/
		if(fcntl(listen_fd, F_SETFL, O_NONBLOCK) < 0)
		{
			std::cerr << "could not set socket to be non blocking" << std::endl;
			exit(1);
		}

	/**========================================================================
	 * ?                         FILL SOCKADDR STRUCT
	 *========================================================================**/
		std::memset((char*)&servaddr, 0, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = INADDR_ANY;
		servaddr.sin_port = htons(port);

	/**========================================================================
	 * ?                         IDENTIFY SOCKET
	 *========================================================================**/
		if (bind(listen_fd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
		{
			std::cerr << "error: bind()" << std::endl;
			exit(1);
		}

	/**========================================================================
	 * ?                         LISTEN SOCKET
	 *========================================================================**/
		if (listen(listen_fd, 100) < 0)
		{
			std::cerr << "error: listen()" << std::endl;
			exit(1);
		}

		return listen_fd;
}

void close_all(vector<int> &sockets, vector<int> &fds_connected, fd_set *r, fd_set * rr)
{
    for (vector<int>::iterator it_s = sockets.begin(); it_s != sockets.end(); it_s++)
    {
        FD_CLR(*it_s, r);
        FD_CLR(*it_s, rr);
        close(*it_s);
    }

    for (vector<int>::iterator it_c = fds_connected.begin(); it_c != fds_connected.end(); it_c++)
        close(*it_c);
}

void get_response(string *response, char *request)
{
    (void)request;
    response->clear();
    *response += "HTTP/1.1 200 OK\n";
    *response += "Content-Type: text/html\r\n";
    *response += "Content-Length: 13\n\n";
    *response += "Hello World !\r\n\r\n";
}

void server(vector<int> ports)
{
    fd_set  fds_read;           //list of set to listen
    fd_set  fds_read_ready;     //list temp use in select
    vector<int> sockets;        //list of socket created
    vector<int> fds_connected;  //list of fds connected with a client
    char request[1024];         //to recv request of client;
	std::string response;       //to send response

    /*===========================================================================*/
    /* create a vector that content all the socket for all the port to listen    */
    /*===========================================================================*/
    for (vector<int>::iterator it_p = ports.begin(); it_p != ports.end(); it_p++)
        sockets.push_back(create_socket(*it_p));
    
    /*====================================*/
    /* config the fd_set to use select    */
    /*====================================*/
    FD_ZERO(&fds_read); 
    for (vector<int>::iterator it_s = sockets.begin(); it_s != sockets.end(); it_s++)
        FD_SET(*it_s, &fds_read);

    /*=============================================*/
    /* loop to accept and communicate with clients */
    /*=============================================*/
    int status = 0;     //return value of select
    int n = 0;
    int max_fds;
    int new_fd = 0;
    struct timeval timeout;
    int ret = 0;
    std::string  wait[] = {"⠋", "⠙", "⠸", "⠴", "⠦", "⠇"};

    /*struct sockaddr_in servaddr;
    size_t servaddr_len = sizeof(servaddr);
    std::memset((char*)&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(4242);*/
    
    while (true)
    {
        status = 0;
        cout << "[ ";
        for (vector<int>::iterator it_c = fds_connected.begin(); it_c != fds_connected.end(); it_c++)
            cout << *it_c << ", ";
        cout << " ]\n";
        while (status == 0)
        {
            max_fds = sockets.back() + 1;
            timeout.tv_sec = 2;
            timeout.tv_usec = 0;
            FD_ZERO(&fds_read_ready);
            for (std::size_t i = 0; i < fds_connected.size(); i++ )
			{
                cout << "wsh";
				if ( fds_connected[i] >= max_fds)
					max_fds = fds_connected[i] + 1;
				FD_SET(fds_connected[i], &fds_read);
			}
            fds_read_ready = fds_read;
            std::cout << "\r" << wait[(n++ % 6)] << GREEN << " waiting for connection" << SET << std::flush;
			usleep(10000);

            //check if a new connection is available
            if ((status = select(max_fds, &fds_read_ready, NULL, NULL, &timeout)) < 0)
            {
                perror("Fail: select() error\n");
                //close_all (sockets, fds_connected);
                exit(EXIT_FAILURE);
            }
        }
        for (vector<int>::iterator it_S = sockets.begin(); it_S != sockets.end(); it_S++)
        {
            if (FD_ISSET(*it_S, &fds_read_ready))
            {
                // create a new fd to comunicate with the client
                if ((new_fd = accept(*it_S, NULL, NULL)) < 0)//reinterpret_cast<sockaddr *>(&servaddr), (socklen_t*)&servaddr_len)) <= 0)
                {
                    if (errno != EWOULDBLOCK)
                    {
                        perror("Fail: accept() error\n");
                        //close_all (sockets, fds_connected);
                        exit(EXIT_FAILURE);
                    }
                }
                else if (new_fd)
                {
                    usleep(1000);
                    //fcntl(new_fd, F_SETFL, O_NONBLOCK);
                    cout << "\r" << "Client connected on server: " << *it_S << endl;
                    FD_SET(new_fd, &fds_read_ready);
                    fds_connected.push_back(new_fd);
                }
            }
            vector<int>::iterator it_c = fds_connected.begin();
            while (it_c != fds_connected.end())
            {
                if (FD_ISSET(*it_c, &fds_read_ready))
                {
                    if ((ret = recv(*it_c, &request, 1023, 0)) < 0)
                    {
                        perror("Fail: recv() error\n");
                        //close_all (sockets, fds_connected);
                        exit(EXIT_FAILURE);
                    }
                    else if (ret == 0)
                    {
                        close(*it_c);
                        fds_connected.erase(it_c);
                        it_c--;
                        std::cout << "Connexion client is closed" << std::endl;
                    }
                    else
                    {
                        request[ret] = '\0';
                        std::cout << "Request: " <<  request << std::endl;
                        //in reality make a response to the request
                        get_response(&response, request);
                        if(ret && (send(*it_c, response.c_str(), response.size(), 0) < 0))
                        {
                            perror("Fail: send() error\n");
                            //close_all (sockets, fds_connected);
                            exit(EXIT_FAILURE);
                        }
                    }
                }
                it_c++;
            }
            
        }

    }
    close_all (sockets, fds_connected, &fds_read, &fds_read_ready);
}

int main(int ac, char **av)
{
    /*=================================================================================*/
    /* for the programme read the config file in parameter and parse the informations. */
    /* here for the test i will give to the programm the different port to listnen     */
    /* i will not secure the data because this is a test                               */
    /*=================================================================================*/
    if (ac < 2)
    {
        std::cerr << "Fail: Usage: ./server [PORT]\n";
        exit(1);
    }
    int i = 1;
    vector<int> ports;
    while (i < ac)
        ports.push_back(atoi(av[i++]));
    server (ports);
    return (0);
}