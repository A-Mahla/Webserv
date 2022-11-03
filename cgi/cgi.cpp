/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meudier <meudier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 08:24:20 by meudier           #+#    #+#             */
/*   Updated: 2022/11/02 15:13:53 by meudier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <sstream>
# include <cstring>

char    **getArgv(std::string script)
{
    char**  argv = new char*[1];
    argv[0] = strdup(script.c_str());
    return (argv);
}

void    clear_argv(char **argv)
{
    free(argv[0]);
    delete [] argv;
}

void    execCGI(void)
{
    int pid;
    std::string     script = "./cgi/test.cgi";
    char            **argv = getArgv(script);
    
    int fd = open("./html/test2.html", O_RDWR | O_CREAT, 0777);
    if (fd < 0)
    {
        std::cout << "error open()" << std::endl;
        return ;
    }
        
    pid = fork();
    if (pid == -1)
    {
        std::cout << "error fork()" << std::endl;
        return ;
    }

    if (pid == 0)
    {
        dup2(fd, STDOUT_FILENO);
        execve(script.c_str(), argv, NULL );
        std::cout << "error execve" << std::endl;
        close(fd);
        clear_argv(argv);
        
    }
    wait(NULL);
    close(fd);
    clear_argv(argv);
}

int main()
{
    execCGI();
}

