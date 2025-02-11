// хз зачем это нужно если честно, вопросы к deepseek
#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <string>

// функция записывает карточку в .csv файл, реализация в flashcard.cpp
bool addFlashcard(const std::string& front, const std::string& back, const std::string& tag);
void displayAllCards();

#endif // тоже хз че это))