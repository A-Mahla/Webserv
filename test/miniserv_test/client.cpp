/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxenceeudier <maxenceeudier@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 16:33:37 by maxenceeudi       #+#    #+#             */
/*   Updated: 2022/10/27 11:46:42 by maxenceeudi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <arpa/inet.h>
#include <cstring>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

int main(int ac, char **av)
{
	struct sockaddr_in	servaddr; //to connect with the server
	int					sock;	  //the socket

	/*========================================================================*/
	/*                         USAGE ERROR MANAGE                             */
	/*========================================================================*/
	if(ac != 3)
	{
		cerr << "usage: ./mini_client [IP] [PORT]" << endl;
		exit(1);
	}

	/*========================================================================*/
	/*                          SOCKET CREATION                               */
	/*========================================================================*/
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		cerr << "error: socket()" << endl;
		exit(1);
	}

	/*========================================================================*/
	/*                          FILL THE SOCKADRR STRUCT                      */
	/*========================================================================*/
    memset(reinterpret_cast<char *>(&servaddr), 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(av[2]));
	servaddr.sin_addr.s_addr = inet_addr(av[1]);

	/*========================================================================*/
	/*                          CONNEXION TO THE ADDRESS                      */
	/*========================================================================*/
	if(connect(sock, reinterpret_cast<sockaddr *>(&servaddr), sizeof(servaddr)) < 0)
	{
		cerr << "error: connect()" << endl;
		exit(1);
	}
    cout << "Conect to server\n";

	/*========================================================================*/
	/*                          CREATE AND SEND HTTP REQUEST                  */
	/*========================================================================*/
	string sendline = "index.html";
	int sendlen = sendline.size();
	int i = 4;

	while (i--)
	{
		int status = 0;
		char buf[1024];
		usleep(50000);
		if (send(sock, sendline.c_str(), sendlen, 0) < 0)
		{
			cerr << "error: send()" <<endl;
			exit(1);
		}
		usleep(50000);
		if ((status = recv(sock, &buf, 1023, 0)) < 0)
		{
			cerr << "error: recv()" <<endl;
			exit(1);
		}
		buf[status] = '\0';
		cout << "I just receve:\n";
		cout << buf << endl;
		usleep(500000);
	}

	/*========================================================================*/
 	/*                         CLOSE THE SOCKET                               */
 	/*========================================================================*/
	close(sock);

	return (0);
}