#include <iostream>
#include <vector>
#include <cstring>


size_t  find(std::vector<unsigned char> str, std::string occur)
{
    size_t  pos = 0;
    size_t  temp;

    for (std::vector<unsigned char>::iterator it = str.begin(); it != str.end(); it++)
    {

        temp = 0;
        if ( *it == occur[temp])
        {
            while (occur[temp] && *(it + temp) == occur[temp])
                temp++;
            if (!occur[temp])
                return (pos);
        }
        pos++;
    }
    return (std::string::npos);
}

void    insert(std::vector<unsigned char> &vec, unsigned char * buff)
{
    int len = strlen((char *)buff);
    int i = 0;
    while (i < len)
        vec.push_back(buff[i++]);
}


int main()
{
    unsigned char buffer[28] = "test 123 test 124 test 1234";
    std::vector<unsigned char> vectorUC;
    
    insert(vectorUC, buffer);

    for (std::vector<unsigned char>::iterator it = vectorUC.begin(); it != vectorUC.end(); it++)
    {
        std::cout << *it;
    }
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "find (123): " << find(vectorUC, "123") << std::endl;
    std::cout << "find (124): " << find(vectorUC, "124") << std::endl;
    std::cout << "find (1234): " << find(vectorUC, "1234") << std::endl;

    if (find(vectorUC, "hey") == std::string::npos)
        std::cout << "i did't find" << std::endl;
    

    return (0);
}