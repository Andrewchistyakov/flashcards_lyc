// хз зачем это нужно если честно, вопросы к deepseek
#ifndef FLASHCARD_H
#define FLASHCARD_H
#define SEP ',' // укажем разделитель чтоб не париться в будущем
#define FILENAME "flashcards.csv"

#include <string>
#include <ncurses.h>

// функция записывает карточку в .csv файл, реализация в flashcard.cpp
bool addFlashcard(const std::string& front, const std::string& back, const std::string& tag);
void displayAllCards();
void displayByTag(const std::string& tag);
bool removeFlashcard(const int wanted_id);
void startReviewAll();
void showHelpMessage();
void startReviewTag(const std::string& tag);
void printCenter(const std::string);
void printCenter(const std::string, const std::string);
void printColor(const std::string, const int);
bool check(const std::string, const std::string);

#endif // тоже хз че это))