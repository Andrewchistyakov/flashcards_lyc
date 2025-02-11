#include "flashcard.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "rapidcsv.h"


// функция получает следующий ID
int getNextID(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        // если файла нету берем айдишник 1
        return 1;
    }

    std::string line, lastLine;
    while (std::getline(file, line)) {
        lastLine = line; // получем последнюю строку 
    }
    file.close();

    if (lastLine.empty()) {
        return 1; // если файлик пустой берем айди 1
    }

    // получаем айди из последней строки
    std::istringstream iss(lastLine);
    std::string idStr;
    std::getline(iss, idStr, ','); // читаем до первой запятой
    try {
        return std::stoi(idStr) + 1; // конвертируем полученную строку в инт и прибавляем 1
    } catch (...) {
        return 1; // если чето не так возвращаем 1
    }
}

// функция записывает карточку в .csv файл
bool addFlashcard(const std::string& front, const std::string& back, const std::string& tag) {

    const std::string filename = "flashcards.csv";

    // считаем какой айди следующий 
    int id = getNextID(filename);
    if (id == -1) {
        std::cerr << "Error: Could not get next ID." << std::endl;
        return false;
    }

    // открываем файл в режиме добавления
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open flashcards.csv for writing." << std::endl;
        return false;
    }
    
    if (id == 1) { // если файл новый, добавим заголовок
        file << "ID,Front,Back,Tag\n";
    }

    // записываем в файл карточку
    file << id <<",\"" << front << "\",\"" << back << "\"" << ",\"" << tag << "\"" << std::endl;

    // закрываем файл
    file.close();

    // возвращем тру типа успешно добавили карточку
    return true;
}

void displayAllCards() {
    const std::string filename = "flashcards.csv";

    // чек на ошибки
    // по факту костыль, хорошо бы переделать
    std::ifstream chck(filename);
    if (!chck.is_open()) {
        std::cerr << "Error: Could not open " << filename << " for reading." << std::endl;
        return;
    } else {
        chck.close();
    }


    // LabelParams в rapidcsv нужны чтобы обозначать парсим значение или идем по номеру
    // 0 значит значение, -1 номер
    rapidcsv::Document file(filename, rapidcsv::LabelParams(0, -1), rapidcsv::SeparatorParams(sep));

    // чут чут красоты
    std::string line;
    std::cout << "Flashcards:\n";
    std::cout << "-----------------------------------\n";

    // читаем файлик и выводим карточки
    for (int i = 0; i < file.GetRowCount(); i++) {
        int id;
        std::string card_front;
        std::string card_back;
        std::string card_tag;

        // парсим строку по частям с либой
        id = file.GetCell<int>("ID", i);
        card_front = file.GetCell<std::string>("Front", i);
        card_back = file.GetCell<std::string>("Back", i);
        card_tag = file.GetCell<std::string>("Tag", i);

        // красиво выводим карточку
        std::cout << "ID: " << id << "\n";
        std::cout << "Front: " << card_front << "\n";
        std::cout << "Back: " << card_back << "\n";
        std::cout << "Tag: " << card_tag << "\n";
        std::cout << "-----------------------------------\n";
    }
}