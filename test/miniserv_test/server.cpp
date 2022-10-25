/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxenceeudier <maxenceeudier@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 10:44:53 by maxenceeudi       #+#    #+#             */
/*   Updated: 2022/10/22 13:04:50 by maxenceeudi      ###   ########.fr       */
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



int create_socket(int port)
{
    int                 listen_fd;
    struct sockaddr_in  address;

    /*============================*/
    /*     create socket          */
    /*============================*/
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0) ) < 0)
    {
        cerr << "Fail: socket() error\n";
        return (0);
    }

    /*================================*/
    /* allowed to reuse addr and port */
    /*================================*/
    int on = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(int)) < 0)
    {
        cerr << "Fail: setsockopt() error\n";
        close (listen_fd);
        return (0);
    }

    /*============================*/
    /*    make non-bloking fd     */
    /*============================*/
    if (fcntl(listen_fd, F_SETFL, O_NONBLOCK) < 0)
    {
        cerr << "Fail: fcntl() error\n";
        close (listen_fd);
        exit(EXIT_FAILURE);
    }

    /*============================*/
    /*       build address        */
    /*============================*/
    memset(reinterpret_cast<char *>(&address), 0, sizeof(address)); 
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    /*============================*/
    /*     bind to an address      */
    /*============================*/
    if (bind(listen_fd, reinterpret_cast<sockaddr *>(&address), sizeof(address)) < 0)
    {
        cerr << "Fail: bind() error\n";
        close (listen_fd);
        return (0);
    }

    /*============================*/
    /*          listen            */
    /*============================*/
    if ( listen(listen_fd, 100) < 0)
    {
        cerr << "Fail: listen() error\n";
        close (listen_fd);
        return (0);
    }

    cout << "A new sockket is listening on port : " << port << endl;
    return (listen_fd);
}

void close_all(vector<int> &sockets, vector<int> &fds_connected, fd_set *r, fd_set * rr)
{
    /*==============================================*/
    /*          CLOSE ALL SOCKETS AND SETS          */
    /*==============================================*/
    for (vector<int>::iterator it_s = sockets.begin(); it_s != sockets.end(); it_s++)
    {
        if (r)
            FD_CLR(*it_s, r);
        if (rr)
            FD_CLR(*it_s, rr);
        close(*it_s);
    }

    /*=======================================================*/
    /*          CLOSE ALL FDS CONNECTED WITH A CLIENT        */
    /*=======================================================*/
    for (vector<int>::iterator it_c = fds_connected.begin(); it_c != fds_connected.end(); it_c++)
        close(*it_c);
}

void get_response(string *response, char *request)
{
    /*=======================================================*/
    /*          TO BUILD A RESPONSE WITH THE REQUEST         */
    /*=======================================================*/
    (void)request;
    response->clear();
    *response += "HTTP/1.1 200 OK\n";
    *response += "Content-Type: text/html\r\n";
    *response += "Content-Length: 13\n\n";
    *response += "Hello World !\r\n\r\n";
}

void    server_select_loop(vector<int> &sockets, vector<int> &fds_connected, fd_set *fds_read, fd_set *fds_read_ready)
{
    int n = 0;                  //just for the wait logo
    std::string  wait[] = {"⠋", "⠙", "⠸", "⠴", "⠦", "⠇"};
    int max_fds;                //for select
    struct timeval timeout;     //for select
    int status = 0;             //return value of select

    while (status == 0)
    {
        max_fds = sockets.back() + 1;
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;
        FD_ZERO(fds_read_ready);
        for (std::size_t i = 0; i < fds_connected.size(); i++ )
        {
            if ( fds_connected[i] >= max_fds)
                max_fds = fds_connected[i] + 1;
            FD_SET(fds_connected[i], fds_read);
        }
        FD_COPY(fds_read, fds_read_ready);
        std::cout << "\r" << wait[(n++ % 6)] << GREEN << " waiting for connection" << SET << std::flush;
        usleep(2000);

        /*=======================================================*/
        /*          check if a new connection is available       */
        /*=======================================================*/

        if ((status = select(max_fds, fds_read_ready, NULL, NULL, &timeout)) < 0)
        {
            perror("Fail: select() error\n");
            close_all (sockets, fds_connected, fds_read, fds_read_ready);
            exit(EXIT_FAILURE);
        }
    }
}

void    server_multi_sockets_loop(vector<int> &sockets, vector<int> &fds_connected, fd_set *fds_read, fd_set *fds_read_ready)
{
    char request[1024];         //to recv request of client;
	std::string response;       //to send response
    int new_fd = 0;             //return of accept 
    int ret = 0;                //return of recv


    /*=============================================================*/
    /* loop to create the fds to connect the server to a client     */
    /*=============================================================*/
    for (vector<int>::iterator it_S = sockets.begin(); it_S != sockets.end(); it_S++)
    {
        if (FD_ISSET(*it_S, fds_read_ready))
        {
            if ((new_fd = accept(*it_S, NULL, NULL)) < 0)
            {
                if (errno != EWOULDBLOCK)
                {
                    perror("Fail: accept() error\n");
                    close_all (sockets, fds_connected, fds_read, fds_read_ready);
                    exit(EXIT_FAILURE);
                }
            }
            if (new_fd)
            {
                cout << "\r" << "Client connected on server: " << *it_S << endl;
                fds_connected.push_back(new_fd);
            }
        }
    }
    
    /*===========================================================*/
    /* loop to send and receve while there is something to read  */
    /*===========================================================*/
    vector<int>::iterator it_c = fds_connected.begin();
    while (it_c != fds_connected.end())
    {
        if (FD_ISSET(*it_c, fds_read_ready))
        {
            /*==================================================*/
            /* create a new fd to comunicat with a new client   */
            /*==================================================*/
            if ((ret = recv(*it_c, &request, 1023, 0)) < 0)
            {
                perror("Fail: recv() error\n");
                close_all (sockets, fds_connected, fds_read, fds_read_ready);
                exit(EXIT_FAILURE);
            }
            else if (ret == 0)
            {
                FD_CLR(*it_c, fds_read);
                close(*it_c);
                fds_connected.erase(it_c);
                it_c--;
                std::cout << "Connexion client is closed" << std::endl;
            }
            else
            {
                request[ret] = '\0';

                /*=======================================================*/
                /* now we have the request we can send the good responce */
                /*=======================================================*/
                std::cout << "Request: " <<  request << std::endl;
                get_response(&response, request);
                if(ret && (send(*it_c, response.c_str(), response.size(), 0) < 0))
                {
                    perror("Fail: send() error\n");
                    close_all (sockets, fds_connected, fds_read, fds_read_ready);
                    exit(EXIT_FAILURE);
                }
            }
        }
        it_c++;
    }
}

void    server_loop(vector<int> &sockets, vector<int> &fds_connected, fd_set *fds_read, fd_set *fds_read_ready)
{
    while (true)
    {
        /*==============================================================*/
        /* just to see the files connected with client to comunicate    */
        /*==============================================================*/
        cout << "[ ";
        for (vector<int>::iterator it_c = fds_connected.begin(); it_c != fds_connected.end(); it_c++)
            cout << *it_c << ", ";
        cout << " ]\n";

        /*==============================================*/
        /* loop while there isn't file to read ready    */
        /*==============================================*/
        server_select_loop(sockets, fds_connected, fds_read, fds_read_ready);

        /*========================================================*/
        /* loop for all sockets to recv() and send() if accept()  */
        /*========================================================*/
        server_multi_sockets_loop(sockets, fds_connected, fds_read, fds_read_ready);
    }
}

void server(vector<int> ports)
{
    fd_set  fds_read;           //list of set to listen
    fd_set  fds_read_ready;     //list temp use in select
    vector<int> sockets;        //list of socket created
    vector<int> fds_connected;  //list of fds connected with a client
    int         fd_listen;      //temp to close all fds if !create_socket()

    /*===========================================================================*/
    /* create a vector that content all the socket for all the port to listen    */
    /*===========================================================================*/
    for (vector<int>::iterator it_p = ports.begin(); it_p != ports.end(); it_p++)
    {
        if ((fd_listen = create_socket(*it_p)))
            sockets.push_back(fd_listen);
        else
            close_all(sockets, fds_connected, NULL , NULL);
    }
        
    /*====================================*/
    /* config the fd_set to use select    */
    /*====================================*/
    FD_ZERO(&fds_read); 
    for (vector<int>::iterator it_s = sockets.begin(); it_s != sockets.end(); it_s++)
        FD_SET(*it_s, &fds_read);

    /*=============================================*/
    /* loop to accept and communicate with clients */
    /*=============================================*/
    server_loop(sockets, fds_connected, &fds_read, &fds_read_ready);

    /*======================*/
    /* close all the fds    */
    /*======================*/
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
        return (0);
    }
    int i = 1;
    vector<int> ports;
    while (i < ac)
        ports.push_back(atoi(av[i++]));
    server (ports);
    return (0);
}