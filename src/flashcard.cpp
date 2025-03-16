#include "flashcard.h"
#include "rapidcsv.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <random>

std::string to_csv_string(
    const std::string id,
    const std::string front,
    const std::string back,
    const std::string tag,
    int successful_guesses,
    int failed_guesses,
    int streak
    ) {
    return id + ",\"" + front + "\",\"" + back + "\",\"" + tag + "\"," + std::to_string(successful_guesses) + "," + std::to_string(failed_guesses) + "," + std::to_string(streak) + "\n";
}

std::string to_csv_string(
    const int id,
    const std::string front,
    const std::string back,
    const std::string tag,
    int successful_guesses,
    int failed_guesses,
    int streak
    ) {
    return std::to_string(id) + ",\"" + front + "\",\"" + back + "\",\"" + tag + "\"," + std::to_string(successful_guesses) + "," + std::to_string(failed_guesses) + "," + std::to_string(streak) + "\n";
}

// функция получает следующий ID
int getNextID(const std::string& fname) {
    try {
        rapidcsv::Document doc(fname, rapidcsv::LabelParams(0, -1), rapidcsv::SeparatorParams(SEP));
        std::vector<int> ids = doc.GetColumn<int>("ID");
    } catch (const std::ifstream::failure &e) {
        return 0; // если файла нет, то возвращаем 0
    } catch (const std::out_of_range &e) {
        return 0; // если файл пуст, то возвращаем 0
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        return -1; // какая-то другая ошибка
    }
    rapidcsv::Document doc(fname, rapidcsv::LabelParams(0, -1), rapidcsv::SeparatorParams(SEP));
    std::vector<int> ids = doc.GetColumn<int>("ID");
    return ids.empty() ? 1 : ids.back() + 1; 
    // если айдишников нет, но заголовок есть, то возвращаем 1, иначе последний + 1
}

// функция записывает карточку в .csv файл
bool addFlashcard(const std::string& front, const std::string& back, const std::string& tag) {
    // считаем какой айди следующий 
    int id = getNextID(FILENAME);
    if (id == -1) {
        std::cerr << "Error: Could not get next ID." << std::endl;
        return false;
    }

    // открываем файл в режиме добавления
    std::ofstream file(FILENAME, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open flashcards.csv for writing." << std::endl;
        return false;
    }
    
    if (id == 0) { // если файл новый, добавим заголовок
        file << "ID,Front,Back,Tag,Successful guesses,Failed guesses,Streak\n";
        id++;
    }

    // записываем в файл карточку
    file << to_csv_string(id, front, back, tag, 0, 0, 0);

    // закрываем файл
    file.close();

    // возвращем тру типа успешно добавили карточку
    return true;
}

bool removeFlashcard(const int wanted_id) {
    // важно отметить, что функция не будет работать 
    // если у нас айдишники в файле не вида 1, 2, 3, 4, 5...
    
    try {
        rapidcsv::Document file(FILENAME, rapidcsv::LabelParams(0, -1), rapidcsv::SeparatorParams(SEP));
    } catch (const std::exception& e) {
        std::cerr << "Error: Could not open " << FILENAME << "for writing." << std::endl << "Error: " << e.what() << std::endl;
        return false;
    }

    int id;
    std::string card_front;
    std::string card_back;
    std::string card_tag;
    int card_successful_guesses;
    int card_failed_guesses;
    int streak;
    std::string line;
    std::vector<std::string> lines;

    rapidcsv::Document file(FILENAME, rapidcsv::LabelParams(0, -1), rapidcsv::SeparatorParams(SEP));
    
    // проверяем есть ли карточка с таким айди
    try {
        file.GetCell<int>("ID", wanted_id);
    } catch (const std::exception& e) {
        std::cerr << "Error: Flashcard with ID " << wanted_id + 1 << " not found." << std::endl << "Error: " << e.what() << std::endl;
        return false;
    }

    // создаем временный файл для записи всего, кроме искомой карточки
    std::ofstream tmpfile("tmp.csv");
    if (!tmpfile.is_open()) {
        std::cerr << "Error: Could not open temporary file for writing." << std::endl;
        return false;
    } else {
        tmpfile << "ID,Front,Back,Tag,Successful guesses,Failed guesses,Streak\n";
    }

    // проходимся по всем карточкам до нашей
    int i;
    for (i = 0; i < wanted_id; ++i) {
        id = file.GetCell<int>("ID", i);
        card_front = file.GetCell<std::string>("Front", i);
        card_back = file.GetCell<std::string>("Back", i);
        card_tag = file.GetCell<std::string>("Tag", i);
        card_successful_guesses = file.GetCell<int>("Successful guesses", i);
        card_failed_guesses = file.GetCell<int>("Failed guesses", i);
        streak = file.GetCell<int>("Streak", i);
        line = to_csv_string(id, card_front, card_back, card_tag, card_successful_guesses, card_failed_guesses, streak);
        tmpfile << line;
    }

    // пропускаем искомую карточку
    i++;

    // добавляем оставшиеся карточки
    for (; i < file.GetRowCount(); ++i) {
        id = file.GetCell<int>("ID", i);
        card_front = file.GetCell<std::string>("Front", i);
        card_back = file.GetCell<std::string>("Back", i);
        card_tag = file.GetCell<std::string>("Tag", i);
        card_successful_guesses = file.GetCell<int>("Successful guesses", i);
        card_failed_guesses = file.GetCell<int>("Failed guesses", i);
        streak = file.GetCell<int>("Streak", i);
        line = to_csv_string(--id, card_front, card_back, card_tag, card_successful_guesses, card_failed_guesses, streak);
        tmpfile << line;
    }
    tmpfile.close();

    if (std::remove(FILENAME)) {
        std::cerr << "Error: Could not remove " << FILENAME << std::endl;
        return false;
    }
    if (std::rename("tmp.csv", FILENAME)) {
        std::cerr << "Error: Could not rename tmp.csv to " << FILENAME << std::endl;
        return false;
    }

    return true;
}

void displayAllCards() {
    // чек на ошибки
    try {
        rapidcsv::Document file(FILENAME, rapidcsv::LabelParams(0, -1), rapidcsv::SeparatorParams(SEP));
    } catch (const std::exception& e) {
        std::cerr << "Error: Could not open " << FILENAME << " for reading." << std::endl << "Error: " << e.what() << std::endl;
        return;
    }

    // LabelParams в rapidcsv нужны чтобы обозначать парсим значение или идем по номеру
    // 0 значит значение, -1 номер
    rapidcsv::Document file(FILENAME, rapidcsv::LabelParams(0, -1), rapidcsv::SeparatorParams(SEP));

    // чут чут красоты
    std::string line;
    std::cout << "Flashcards:\n";
    std::cout << "-----------------------------------\n";


    // читаем файлик и выводим карточки
    int id;
    std::string card_front;
    std::string card_back;
    std::string card_tag;
    std::string card_stats;
    int streak;
    for (int i = 0; i < file.GetRowCount(); i++) {
        // парсим строку по частям с либой
        id = file.GetCell<int>("ID", i);
        card_front = file.GetCell<std::string>("Front", i);
        card_back = file.GetCell<std::string>("Back", i);
        card_tag = file.GetCell<std::string>("Tag", i);
        card_stats = "Sucessful guesses: " + file.GetCell<std::string>("Successful guesses", i) + "\nFailed guesses: " + file.GetCell<std::string>("Failed guesses", i);
        streak = file.GetCell<int>("Streak", i);

        // красиво выводим карточку
        std::cout << "ID: " << id << "\n";
        std::cout << "Front: " << card_front << "\n";
        std::cout << "Back: " << card_back << "\n";
        std::cout << "Tag: " << card_tag << "\n";
        std::cout << card_stats << "\n";
        std::cout << "Current streak: " << streak << '\n';
        std::cout << "-----------------------------------\n";
    }
}

void displayByTag(const std::string& tag){
    try {
        rapidcsv::Document file(FILENAME, rapidcsv::LabelParams(0, -1), rapidcsv::SeparatorParams(SEP));
    } catch (const std::exception& e) {
        std::cerr << "Error: Could not open " << FILENAME << " for reading." << std::endl << "Error: " << e.what() << std::endl;
        return;
    }

    rapidcsv::Document file(FILENAME, rapidcsv::LabelParams(0, -1), rapidcsv::SeparatorParams(SEP));

    // чут чут красоты
    std::string line;
    std::cout << "Flashcards: by tag \"" << tag << "\"\n";
    std::cout << "-----------------------------------\n";


    // читаем файлик и выводим карточки
    int id;
    std::string card_front;
    std::string card_back;
    std::string card_tag;
    std::string card_stats;
    int streak;

    for (int i = 0; i < file.GetRowCount(); i++) {
        // парсим строку по частям с либой
        id = file.GetCell<int>("ID", i);
        card_front = file.GetCell<std::string>("Front", i);
        card_back = file.GetCell<std::string>("Back", i);
        card_tag = file.GetCell<std::string>("Tag", i);
        card_stats = "Sucessful guesses: " + file.GetCell<std::string>("Successful guesses", i) + "\nFailed guesses: " + file.GetCell<std::string>("Failed guesses", i);
        streak = file.GetCell<int>("Streak", i);
        if (card_tag == tag) {
            // красиво выводим карточку
            std::cout << "ID: " << id << "\n";
            std::cout << "Front: " << card_front << "\n";
            std::cout << "Back: " << card_back << "\n";
            std::cout << "Tag: " << card_tag << "\n";
            std::cout << card_stats << "\n";
            std::cout << "Current streak:" << streak << '\n';
            std::cout << "-----------------------------------\n";
        }
    }
}

void startReviewAll() {
    // чек на ошибки
    try {
        rapidcsv::Document file(FILENAME, rapidcsv::LabelParams(0, -1), rapidcsv::SeparatorParams(SEP));
    } catch (const std::exception& e) {
        std::cerr << "Error: Could not open " << FILENAME << " for reading." << std::endl << "Error: " << e.what() << std::endl;
        return;
    }

    rapidcsv::Document file(FILENAME, rapidcsv::LabelParams(0, -1), rapidcsv::SeparatorParams(SEP));

    std::string card_front;
    std::string card_back;
    std::string user_response;
    int successful_guesses;
    int failed_guesses;
    int streak;

    // shuffling cards
    std::vector<int> needed_ids;
    for (int i = 0; i < file.GetRowCount(); ++i) {
        int streak = file.GetCell<int>("Streak", i);
        if (streak < 10) {
            needed_ids.push_back(i);
        }
    }
    
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(needed_ids.begin(),needed_ids.end(), rng);

    
    
    for (int i : needed_ids) {
        // получаем из файлика карточку
        card_front = file.GetCell<std::string>("Front", i);
        card_back = file.GetCell<std::string>("Back", i);
        streak = file.GetCell<int>("Streak", i);

        displayFront(card_front);
        std::string user_response = displayBack(card_back);
        if (user_response == "y" || user_response == "Y") {
            displayTextColor("Good job!", ftxui::Color::GreenLight);
            // writing stats
            successful_guesses = file.GetCell<int>("Successful guesses", i);
            successful_guesses++;
            streak++;
            file.SetCell<int>("Successful guesses", i, successful_guesses);
            file.SetCell<int>("Streak", i, streak);
        } else {
            displayTextColor("Try to memorize it!", ftxui::Color::RedLight);
            // writing stats
            failed_guesses = file.GetCell<int>("Failed guesses", i);
            failed_guesses++;
            streak = 0;
            file.SetCell<int>("Failed guesses", i, failed_guesses);
            file.SetCell<int>("Streak", i, streak);
        }
    }
    displayText("You finished them all");
    file.Save("flashcards.csv");
}

void startReviewTag(const std::string& tag) {
    // чек на ошибки
    try {
        rapidcsv::Document file(FILENAME, rapidcsv::LabelParams(0, -1), rapidcsv::SeparatorParams(SEP));
    } catch (const std::exception& e) {
        std::cerr << "Error: Could not open " << FILENAME << " for reading." << std::endl << "Error: " << e.what() << std::endl;
        return;
    }

    rapidcsv::Document file(FILENAME, rapidcsv::LabelParams(0, -1), rapidcsv::SeparatorParams(SEP));

    std::string card_front;
    std::string card_back;
    std::string card_tag;
    int successful_guesses;
    int failed_guesses;
    int streak;

    // shuffling cards
    std::vector<int> needed_ids;
    for (int i = 0; i < file.GetRowCount(); ++i) {
        std::string current_tag = file.GetCell<std::string>("Tag", i);
        int streak = file.GetCell<int>("Streak", i);
        if (current_tag == tag && streak < 10) {
            needed_ids.push_back(i);
        }
    }
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(needed_ids.begin(),needed_ids.end(), rng);

    std::string user_response;
    for (int i : needed_ids) {
        // получаем из файлика карточку
        card_front = file.GetCell<std::string>("Front", i);
        card_back = file.GetCell<std::string>("Back", i);
        card_tag = file.GetCell<std::string>("Tag", i);
        streak = file.GetCell<int>("Streak", i);

        if (card_tag != tag) {
            continue;
        }

        displayFront(card_front);
        std::string user_response = displayBack(card_back);
        // выводим бэк
        if (user_response == "y" || user_response == "Y") { // мб функу для чека сделать
            displayTextColor("Good job!", ftxui::Color::GreenLight); // green = right
            // writing stats
            successful_guesses = file.GetCell<int>("Successful guesses", i);
            successful_guesses++;
            streak++;
            file.SetCell<int>("Successful guesses", i, successful_guesses);
            file.SetCell<int>("Streak", i, streak);
        } else {
            displayTextColor("Try to memorize it!", ftxui::Color::RedLight); // red = wrong
            // writing stats
            failed_guesses = file.GetCell<int>("Failed guesses", i);
            failed_guesses++;
            streak = 0;
            file.SetCell<int>("Failed guesses", i, failed_guesses);
            file.SetCell<int>("Streak", i, streak);
        }
    }
    file.Save();
    displayText("You finished them all!");
}

void showStats(std::string tag) {
    // чек на ошибки
    try {
        rapidcsv::Document file(FILENAME, rapidcsv::LabelParams(0, -1), rapidcsv::SeparatorParams(SEP));
    } catch (const std::exception& e) {
        std::cerr << "Error: Could not open " << FILENAME << " for reading." << std::endl << "Error: " << e.what() << std::endl;
        return;
    }

    rapidcsv::Document file(FILENAME, rapidcsv::LabelParams(0, -1), rapidcsv::SeparatorParams(SEP));

    std::string card_front;
    std::string card_back;
    std::string card_tag;
    int successful_guesses;
    int failed_guesses;
    int streak;

    for (int i = 0; i < file.GetRowCount(); i++) {
        // получаем из файла карточку
        card_front = file.GetCell<std::string>("Front", i);
        card_back = file.GetCell<std::string>("Back", i);
        card_tag = file.GetCell<std::string>("Tag", i);
        successful_guesses = file.GetCell<int>("Successful guesses", i);
        failed_guesses = file.GetCell<int>("Failed guesses", i);
        streak = file.GetCell<int>("Streak", i);

        if (card_tag != tag) {
            continue;
        }

        std::string progressBar = "[";

        if (streak > 10) {
            streak = 10;
        }   
        
        for (int j = 0; j < streak; j++) {
            progressBar += "#";
        }
        for (int j = 0; j < 10 - streak; j++) {
            progressBar += "-";
        }
        progressBar += "] " + std::to_string(streak * 10) + "%";


        std::cout << "Front: " << card_front << "\n";
        std::cout << "Back: " << card_back << "\n";
        std::cout << "Successful guesses: " << successful_guesses << "\n";
        std::cout << "Failed guesses: " << failed_guesses << "\n";
        std::cout << "Learning progress: " << progressBar << "\n";
        std::cout << "-----------------------------------\n";
    }
}

void showHelpMessage() {
    std::cout << "--------------------------- FCARD ---------------------------" << std::endl;
    std::cout << "\"./fcard privet\" - prints \"Hello world!\"" << std::endl;
    std::cout << "\"./fcard list\" - prints all flashcards" << std::endl;
    std::cout << "\"./fcard add <card_front> <card_back>\" - adds a new card" << std::endl;
    std::cout << "\"./fcard remove <card_id>\" - removes selected card" << std::endl;
    std::cout << "\"./fcard review -a\" - starts review mode" << std::endl;
}