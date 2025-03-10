// хз зачем это нужно если честно, вопросы к deepseek
#ifndef FLASHCARD_H
#define FLASHCARD_H
#define SEP ',' // укажем разделитель чтоб не париться в будущем
#define FILENAME "flashcards.csv"

#include <string>
#include <iostream>

#include <memory>  // for allocator, __shared_ptr_access
#include <string>  // for char_traits, operator+, string, basic_string

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for InputOption
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/util/ref.hpp"  // for Ref

// функция записывает карточку в .csv файл, реализация в flashcard.cpp
bool addFlashcard(const std::string& front, const std::string& back, const std::string& tag);
void displayAllCards();
void displayByTag(const std::string& tag);
bool removeFlashcard(const int wanted_id);
void startReviewAll();
void showHelpMessage();
void displayText(std::string);
void displayTextColor(std::string, ftxui::Color fron_color = ftxui::Color::Default, ftxui::Color back_color = ftxui::Color::Default);
std::string displayFront(std::string);
std::string displayBack(std::string);
void startReviewTag(const std::string& tag);
/*void printCenter(const std::string);
void printCenter(const std::string, const std::string);
void printColor(const std::string, const int);
bool check(const std::string, const std::string);*/

#endif // тоже хз че это))