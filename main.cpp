#include <iostream>
#include "argh.h"
#include "flashcard.h"

int main(int, char* argv[])
{
    // cmdl - парсер из "Argh!", из него получаем аргументы
    argh::parser cmdl(argv);

    // тестовая команда привет, по ней можно будет проверять, запускается ли вообще программа
    if (cmdl[1] == "privet") {
        std::cout << "Hello world!\n" << std::endl;
    } else if (cmdl[1] == "add") {
        // убеждаемся что юзер ввел достаточно аргументов для добавления карточки
        if (cmdl.size() < 4) {
            std::cerr << "to add a card you need to type: fcard add <front> <back>" << std::endl;
            return 1;
        }

        // забираем перед и зад карточки из аргументов
        std::string front = cmdl[2];
        std::string back = cmdl[3];

        // записываем в файлик
        if (addFlashcard(front, back)) {
            std::cout << "Flashcard added successfully!" << std::endl;
        } else {
            std::cerr << "Failed to add flashcard." << std::endl;
        }
    } else if (cmdl[1] == "list" && !cmdl(2)) {
        displayAllCards();
    } else {
        std::cerr << "Unknown command. Available commands: privet, add" << std::endl;
    }

    return 0;
}