#include "flashcard.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "rapidcsv.h"

std::string to_csv_string(const std::string id, const std::string front, const std::string back, const std::string tag) {
    return id + ",\"" + front + "\",\"" + back + "\",\"" + tag + "\"\n";
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
        file << "ID,Front,Back,Tag\n";
        id++;
    }

    // записываем в файл карточку
    file << to_csv_string(std::to_string(id), front, back, tag);

    // закрываем файл
    file.close();

    // возвращем тру типа успешно добавили карточку
    return true;
}

bool removeFlashcard(int wanted_id) {
    // важно отметить, что функция не будет работать 
    // если у нас айдишники в файле не вида 1, 2, 3, 4, 5...

    wanted_id--; // уменьшаем на 1, тк в файле айдишники начинаются с 1
    
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
        tmpfile << "ID,Front,Back,Tag\n";
    }

    // проходимся по всем карточкам до нашей
    int i;
    for (i = 0; i < wanted_id; ++i) {
        id = file.GetCell<int>("ID", i);
        card_front = file.GetCell<std::string>("Front", i);
        card_back = file.GetCell<std::string>("Back", i);
        card_tag = file.GetCell<std::string>("Tag", i);
        line = to_csv_string(std::to_string(id), card_front, card_back, card_tag);
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
        line = to_csv_string(std::to_string(--id), card_front, card_back, card_tag);
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
