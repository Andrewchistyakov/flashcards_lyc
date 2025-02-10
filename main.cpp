#include <iostream>
#include "argh.h"

int main(int, char* argv[])
{
    argh::parser cmdl(argv);

    if (cmdl[1] == "privet") {
        std::cout << "Hello world!\n";
    } else {
        std::cout << "type \"privet\"" << std::endl;
    }
        

    return EXIT_SUCCESS;
}