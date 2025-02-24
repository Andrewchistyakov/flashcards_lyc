#include "flashcard.h"
#include "rapidcsv.h"
#include <fstream>
#include <iostream>
#include <sstream>

std::string to_csv_string(
    const std::string id,
    const std::string front,
    const std::string back,
    const std::string tag,
    int successful_guesses,
    int failed_guesses
    ) {
    return id + ",\"" + front + "\",\"" + back + "\",\"" + tag + "\"," + std::to_string(successful_guesses) + "," + std::to_string(failed_guesses) + "\n";
}

std::string to_csv_string(
    const int id,
    const std::string front,
    const std::string back,
    const std::string tag,
    int successful_guesses,
    int failed_guesses
    ) {
    return std::to_string(id) + ",\"" + front + "\",\"" + back + "\",\"" + tag + "\"," + std::to_string(successful_guesses) + "," + std::to_string(failed_guesses) + "\n";
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
        file << "ID,Front,Back,Tag,Successful guesses,Failed guesses\n";
        id++;
    }

    // записываем в файл карточку
    file << to_csv_string(id, front, back, tag, 0, 0);

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
        tmpfile << "ID,Front,Back,Tag,Successful guesses,Failed guesses\n";
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
        line = to_csv_string(id, card_front, card_back, card_tag, card_successful_guesses, card_failed_guesses);
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
        line = to_csv_string(--id, card_front, card_back, card_tag, card_successful_guesses, card_failed_guesses);
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
    for (int i = 0; i < file.GetRowCount(); i++) {
        // парсим строку по частям с либой
        id = file.GetCell<int>("ID", i);
        card_front = file.GetCell<std::string>("Front", i);
        card_back = file.GetCell<std::string>("Back", i);
        card_tag = file.GetCell<std::string>("Tag", i);
        card_stats = "Sucessful guesses: " + file.GetCell<std::string>("Successful guesses", i) + "\nFailed guesses: " + file.GetCell<std::string>("Failed guesses", i);

        // красиво выводим карточку
        std::cout << "ID: " << id << "\n";
        std::cout << "Front: " << card_front << "\n";
        std::cout << "Back: " << card_back << "\n";
        std::cout << "Tag: " << card_tag << "\n";
        std::cout << card_stats << "\n";
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
    for (int i = 0; i < file.GetRowCount(); i++) {
        // парсим строку по частям с либой
        id = file.GetCell<int>("ID", i);
        card_front = file.GetCell<std::string>("Front", i);
        card_back = file.GetCell<std::string>("Back", i);
        card_tag = file.GetCell<std::string>("Tag", i);
        if (card_tag == tag) {
            // красиво выводим карточку
            std::cout << "ID: " << id << "\n";
            std::cout << "Front: " << card_front << "\n";
            std::cout << "Back: " << card_back << "\n";
            std::cout << "Tag: " << card_tag << "\n";
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
    for (int i = 0; i < file.GetRowCount(); i++) {
        // получаем из файлика карточку
        card_front = file.GetCell<std::string>("Front", i);
        card_back = file.GetCell<std::string>("Back", i);

        // выводим фронт
        std::cout << card_front << std::endl;
        std::cout << "Type anything to show answer" << std::endl << "> ";
        std::cin >> user_response;
        // выводим бэк
        std::cout << "Answer: " << card_back << std::endl;
        std::cout << "press \"y\" if you got it and \"n\" if not" << std::endl << "> ";
        std::cin >> user_response;

        if (user_response == "y" || user_response == "Y") {
            std::cout << "Good job!\n-----------------------------------------------------" << std::endl;
            // writing stats
            successful_guesses = file.GetCell<int>("Successful guesses", i);
            successful_guesses++;
            file.SetCell<int>("Successful guesses", i, successful_guesses);
        } else {
            std::cout << "Try to memorize it!\n-----------------------------------------------------" << std::endl;
            // writing stats
            failed_guesses = file.GetCell<int>("Failed guesses", i);
            failed_guesses++;
            file.SetCell<int>("Failed guesses", i, failed_guesses);
        }

        if (i == file.GetRowCount() - 1) {
            std::cout << "--------------- You finished them all! --------------" << std::endl; 
        }
    }
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

    std::string user_response;
    for (int i = 0; i < file.GetRowCount(); i++) {
        // получаем из файлика карточку
        card_front = file.GetCell<std::string>("Front", i);
        card_back = file.GetCell<std::string>("Back", i);
        card_tag = file.GetCell<std::string>("Tag", i);

        if (card_tag != tag) {
            continue;
        }

        // выводим фронт
        std::cout << card_front << std::endl;
        std::cout << "Type anything to show answer" << std::endl << "> ";
        std::cin >> user_response;
        // выводим бэк
        std::cout << "Answer: " << card_back << std::endl;
        std::cout << "press \"y\" if you got it and \"n\" if not" << std::endl  << "> ";

        std::cin >> user_response;

        if (user_response == "y" || user_response == "Y") {
            std::cout << "Good job!\n-----------------------------------------------------" << std::endl;
            // writing stats
            successful_guesses = file.GetCell<int>("Successful guesses", i);
            successful_guesses++;
            file.SetCell<int>("Successful guesses", i, successful_guesses);
        } else {
            std::cout << "Try to memorize it!\n-----------------------------------------------------" << std::endl;
            // writing stats
            failed_guesses = file.GetCell<int>("Failed guesses", i);
            failed_guesses++;
            file.SetCell<int>("Failed guesses", i, failed_guesses);
        }
    }
    file.Save();
    
    std::cout << "--------------- You finished them all! --------------" << std::endl;
}

void showHelpMessage() {
    std::cout << "--------------------------- FCARD ---------------------------" << std::endl;
    std::cout << "\"./fcard privet\" - prints \"Hello world!\"" << std::endl;
    std::cout << "\"./fcard list\" - prints all flashcards" << std::endl;
    std::cout << "\"./fcard add <card_front> <card_back>\" - adds a new card" << std::endl;
    std::cout << "\"./fcard remove <card_id>\" - removes selected card" << std::endl;
    std::cout << "\"./fcard review -a\" - starts review mode" << std::endl;
}
