#include "flashcard.h"
#include <fstream>
#include <iostream>
#include <sstream>

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
        return -1; // если чето не так возвращаем -1 как знак ошибки
    }
}

// функция записывает карточку в .csv файл
bool addFlashcard(const std::string& front, const std::string& back) {

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

    // записываем в файл карточку
    file << id <<",\"" << front << "\",\"" << back << "\"" << std::endl;

    // закрываем файл
    file.close();

    // возвращем тру типа успешно добавили карточку
    return true;
}

void displayAllCards() {

    const std::string filename = "flashcards.csv";

    // открываем файл и чекаем на ошибки
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << " for reading." << std::endl;
        return;
    }

    // чут чут красоты
    std::string line;
    std::cout << "Flashcards:\n";
    std::cout << "-----------------------------------\n";

    // читаем файлик и выводим карточки
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string id;
        std::string card_front;
        std::string card_back;

        // парсим строку разделяем по частям
        std::getline(iss, id, ',');
        std::getline(iss, card_front, ',');
        std::getline(iss, card_back, ',');

        // убираем некрасивые кавычки
        if (!card_front.empty() && card_front.front() == '"') card_front.erase(0, 1);
        if (!card_front.empty() && card_front.back() == '"') card_front.pop_back();
        if (!card_back.empty() && card_back.front() == '"') card_back.erase(0, 1);
        if (!card_back.empty() && card_back.back() == '"') card_back.pop_back();

        // красиво выводим карточку
        std::cout << "ID: " << id << "\n";
        std::cout << "Front: " << card_front << "\n";
        std::cout << "Back: " << card_back << "\n";
        std::cout << "-----------------------------------\n";
    }

    // Close the file
    file.close();
}