#ifndef MENU_H
#define MENU_H

#include "LCDScreen.h"

#define MENU_LEN 10

class LCDScreen;

enum MenuID {
    VOICE,
    PITCH,
    OSCILLATORS,
    MIXER,
    FILTER,
    FILTER_ENV,
    AMP,
    AMP_ENV,
    LFO1,
    LFO2
};

class MenuOption {
public:
    MenuOption(const char* _name, MenuID _id);
    MenuID id;
    int get_p1() {
        return 10;
    }
    int get_p2() {
        return 20;
    }
    int get_p3() {
        return 30;
    }
    int get_p4() {
        return 40;
    }
    int get_p5() {
        return 50;
    }
    char name[20];
};

class Menu {
public:
    MenuOption menu[MENU_LEN] = {
        MenuOption("Voice", MenuID::VOICE),
        MenuOption("Pitch", MenuID::PITCH),
        MenuOption("Oscillators", MenuID::OSCILLATORS),
        MenuOption("Mixer", MenuID::MIXER),
        MenuOption("Filter", MenuID::FILTER),
        MenuOption("Filter Envelope", MenuID::FILTER_ENV),
        MenuOption("Amp", MenuID::AMP),
        MenuOption("Amp Envelope", MenuID::AMP_ENV),
        MenuOption("LFO 1", MenuID::LFO1),
        MenuOption("LFO 2", MenuID::LFO2)
    };
    LCDScreen* lcd;
    int menu_index = 0;
    void increment_index(int inc);
    void navigate(int reading);
    void select();
    void init(LCDScreen* _lcd);
};

#endif