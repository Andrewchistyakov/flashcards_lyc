#include <iostream>
#include "argh.h"

int main(int, char* argv[])
{
    // cmdl - парсер из "Argh!", из него получаем аргументы
    argh::parser cmdl(argv);

    // тестовая команда привет, по ней можно будет проверять, запускается ли вообще программа
    if (cmdl[1] == "privet") {
        std::cout << "Hello world!\n";
    } else {
        std::cout << "type \"privet\"" << std::endl;
    }
        

    return EXIT_SUCCESS;
}