#include "Menu.h"
#include "cstring"

MenuOption::MenuOption(char* _name) {
    strcpy(name, _name);    
}

void Menu::increment_index(int inc) {
    if(inc > 0) {
        menu_index = (menu_index + 1) % MENU_LEN;
    }
    else if(inc < 0) {
        menu_index -= 1;
        if(menu_index < 0)
            menu_index = MENU_LEN - 1;
    }
}