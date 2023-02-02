#include "CursesScreen.h"

void CursesScreen::init() {
    initscr();
    noecho();
    printw("Starting up screen...");
    refresh();
    getch();
}

void CursesScreen::send_string(char *str) {
    printw("%s", str);
}

void CursesScreen::put_cur(int row, int col) {
	move(row, col);
}

void CursesScreen::cursor_setup(bool cur_on, bool blink_on) {
    //do nothing
}

void CursesScreen::clear() {
    erase();
}
