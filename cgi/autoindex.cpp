#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>

char**	buildCGIenv(void);

int	exec_ls(int *fds, char* const* ls_cmd){
		close(fds[0]);
		dup2(fds[1], 1);
		close(fds[1]);
		return (execve(ls_cmd[0], ls_cmd, NULL) < 0);
}

int get_ls_output(int *fds, std::string & ls_output){
	char	buf[1024];
	int		rd;

	close(fds[1]);
	wait(NULL);
	while ((rd = read(fds[0], buf, 1023)) != 0){
		buf[rd] = '\0';
		ls_output += buf;
	}
	close(fds[0]);
	return (0);
}

void	listDir(std::string & ls_output, const char *path){
	char *ls_cmd[4];
	int pid = 0;
	int fds[2];

	ls_cmd[0] = static_cast<char *>(malloc(strlen("/bin/ls") + 1));
	strcpy(ls_cmd[0], "/bin/ls");
	ls_cmd[1] = static_cast<char *>(malloc(strlen("-p") + 1));
	strcpy(ls_cmd[1], "-p");
	ls_cmd[2] = static_cast<char *>(malloc(strlen(path) + 1));
	strcpy(ls_cmd[2], path);
	ls_cmd[3] = NULL;
	if (pipe(fds) < 0)
		return ;
	pid = fork();
	if (pid == 0) {
		if (exec_ls(fds, ls_cmd) < 0)
			std::cout << "**error execve !!!**\n";
	}
	else if (pid)
		get_ls_output(fds, ls_output);
	free(ls_cmd[0]);
	free(ls_cmd[1]);
	free(ls_cmd[2]);
}

void	find_path(std::string & path, char **env){
	int i = 0;
	size_t equal;
	size_t pathlen;

	for (; *(env + i) && strncmp(*(env + i), "PATH_TRANSLATED=", strlen("PATH_TRANSLATED=")); i++);
	if ( !*(env + i) )
		return ;
	path = *(env + i);
	equal = path.find("=", 0);
	pathlen = path.size();
	path = path.substr(equal + 1, pathlen - (equal + 2));
	if (path.find("./", 0) == 0)
		path.insert(0, ".");
	else if (path.find("/", 0) == 0)
		path.insert(0, "..");
	else
		path.insert(0, "../");
}


int main(int ac, char **av, char **env){
	(void) ac;
	(void) av;
	(void) env;
	std::string path;
	std::string ls_output;
	std::vector<std::string> tab_output;
	char **testenv = buildCGIenv();

	find_path(path, testenv);
	for (int i = 0; *(testenv + i); i++){
		free(*(testenv + i));
	}
	listDir(ls_output, path.c_str());
	tab_output.push_back(ls_output.substr(0, (ls_output.find("\n", 0) - (0))));
	for (size_t pos = 0; (pos = ls_output.find("\n", pos)) != std::string::npos; ){
		pos++;
		tab_output.push_back(ls_output.substr(pos, (ls_output.find("\n", pos) - (pos))));
	}
	tab_output.pop_back();
	std::cout << "la taille de mon vecteur : " << tab_output.size() << "\n";
	for (std::vector<std::string>::iterator it = tab_output.begin(); it != tab_output.end(); it++)
	{
		std::cout << *it << "\n";
	}
	free(testenv);
	return (0);
}
