// хз зачем это нужно если честно, вопросы к deepseek
#ifndef FLASHCARD_H
#define FLASHCARD_H
#define SEP ',' // укажем разделитель чтоб не париться в будущем
#define FILENAME "flashcards.csv"

#include <string>

// функция записывает карточку в .csv файл, реализация в flashcard.cpp
bool addFlashcard(const std::string& front, const std::string& back, const std::string& tag);
void displayAllCards();
bool removeFlashcard(const int wanted_id);
void startReviewAll();
void showHelpMessage();
void startReviewTag(const std::string& tag);

#endif // тоже хз че это))