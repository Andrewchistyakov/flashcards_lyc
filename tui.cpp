#include "flashcard.h"


void init() {
    initscr();
    noecho();
    curs_set(FALSE);
}

void center(const std::string text) {
    clear();
    box(stdscr, 0, 0);
    int row, col;
    getmaxyx(stdscr, row, col);
    int center_row = row / 2;
    int center_col = (col - text.length()) / 2;
    mvprintw(center_row, center_col, text.data());
    refresh();
}

void center(const std::string a, const std::string b) {
    clear();
    box(stdscr, 0, 0);
    int row, col;
    getmaxyx(stdscr, row, col);
    int center_row = (row / 2);
    int center_col_first = (col - a.length()) / 2;
    mvprintw(center_row, center_col_first, a.data());
    int center_col_second = (col - b.length()) / 2;
    mvprintw(center_row + 1, center_col_second, b.data());
    refresh();
}

bool check(const std::string a, const std::string b) {
    initscr();
    echo();
    curs_set(true);
    clear();
    box(stdscr, 0, 0);
    int row, col;
    getmaxyx(stdscr, row, col);
    center(a, b);

    // move down the cursor
    // по y: row нельзя, row - 1 на рамку залезает => row -2 
    // по x: 0 на рамку залезает => 1
    move(row - 2, 1);
    refresh();
    
    char c[2];
    getnstr(c, 1);
    endwin();
    return c[0] == 'y' || c[0] == 'Y' ? 1 : 0;
}

void printCenter(const std::string text) {
    init();
    clear();
    box(stdscr, 0, 0);
    int row, col;
    getmaxyx(stdscr, row, col);
    int center_row = row / 2;
    int center_col = (col - text.length()) / 2;
    mvprintw(center_row, center_col, text.data());
    refresh();
    getch();
    endwin();
}

void printCenter(const std::string a, const std::string b) {
    init();
    clear();
    box(stdscr, 0, 0);
    int row, col;
    getmaxyx(stdscr, row, col);
    int center_row = (row / 2);
    int center_col_first = (col - a.length()) / 2;
    mvprintw(center_row, center_col_first, a.data());
    int center_col_second = (col - b.length()) / 2;
    mvprintw(center_row + 1, center_col_second, b.data());
    refresh();
    getch();
    endwin();
}

void printColor(const std::string text, const int color) {
    init();
    start_color();
    use_default_colors();
    init_pair(1, color, -1); 
    clear();

    attron(COLOR_PAIR(1));
    center(text);
    attroff(COLOR_PAIR(1));
    refresh();

    getch();
    endwin();
}