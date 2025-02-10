#include "flashcard.h"
#include <fstream>
#include <iostream>

// функция записывает карточку в .csv файл
bool addFlashcard(const std::string& front, const std::string& back) {
    // открываем файл в режиме добавления
    std::ofstream file("flashcards.csv", std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open flashcards.csv for writing." << std::endl;
        return false;
    }

    // записываем в файл карточку
    file << "\"" << front << "\",\"" << back << "\"" << std::endl;

    // закрываем файл
    file.close();

    // возвращем тру типа успешно добавили карточку
    return true;
}