#ifndef MENU_H
#define MENU_H

#define MENU_LEN 10

class MenuOption {
public:
    MenuOption(char* _name);
    char name[20];
};

class Menu {
public:
    MenuOption menu[MENU_LEN] = {
        MenuOption("Voice"),
        MenuOption("Pitch"),
        MenuOption("Oscillators"),
        MenuOption("Mixer"),
        MenuOption("Filter"),
        MenuOption("Filter Envelope"),
        MenuOption("Amp"),
        MenuOption("Amp Envelope"),
        MenuOption("LFO 1"),
        MenuOption("LFO 2")
    };
    int menu_index = 0;
    void increment_index(int inc);
    
};

#endif