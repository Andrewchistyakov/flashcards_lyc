#include <iostream>
#include "argh.h"
#include "flashcard.h"

int main(int, char* argv[])
{
    // cmdl - парсер из "Argh!", из него получаем аргументы
    argh::parser cmdl({"-t", "--tag", "-a", "--all"});
    cmdl.parse(argv);

    // тестовая команда привет, по ней можно будет проверять, запускается ли вообще программа
    if (cmdl[1] == "privet") {
        std::cout << "Hello world!\n" << std::endl;
    } else if (cmdl[1] == "add") {
        // убеждаемся что юзер ввел достаточно аргументов для добавления карточки
        if (cmdl.size() < 4) {
            std::cerr << "to add a card you need to type: fcard add <front> <back>" << std::endl;
            return 1;
        }

        std::string tag;
        if (!(cmdl({"-t", "--tag"}) >> tag)) {
            std::cerr << "You have to specify the tag for the flashcard.\nUse \"-t\" or \"--tag\" followed by tag name" << std::endl;
            return 1;
        }
        // забираем перед и зад карточки из аргументов
        std::string front = cmdl[2];
        std::string back = cmdl[3];

        // записываем в файлик
        if (addFlashcard(front, back, tag)) {
            std::cout << "Flashcard added successfully!" << std::endl;
        } else {
            std::cerr << "Failed to add flashcard." << std::endl;
        }
    } else if (cmdl[1] == "list" && !cmdl(2)) {
        displayAllCards();
    } else if (cmdl[1] == "remove" && cmdl(2)) {
        // удаляем карточку
        if (removeFlashcard(std::stoi(cmdl[2]))) {
            std::cout << "Flashcard removed successfully!" << std::endl;
        } else {
            std::cerr << "Failed to remove flashcard." << std::endl;
            return 1;
        }
    } else if (cmdl[1] == "review") {
        if (cmdl[{"-a", "--all"}]) {
            startReviewAll();
        }
    } else if (cmdl[1] == "help") {
        showHelpMessage();
    } else {
        std::cerr << "Unknown command. Available commands: privet, list, add, remove" << std::endl;
    }

    return 0;
}