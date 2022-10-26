#include <iostream>
#include <string>

bool onlyDiggit(std::string test){
	std::cout << "what i received: " << test << std::endl;
	return (true);
}

int main(){

	std::string addr("127.0.0.789");
	size_t	occurence = 0;
	int	pos = 0;

	while ( ( occurence = addr.find(".", occurence) ) != std::string::npos){
		if (!onlyDiggit(addr.substr(pos, occurence - pos)))
			return (false);
		occurence++;
		pos = occurence;
	}
	if (!onlyDiggit(addr.substr(pos, occurence - pos)))
		return (false);


	return (0);
}
