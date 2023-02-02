#ifndef MENU_H
#define MENU_H

#include "LCDScreen.h"
#include "PatchParams.h"
#include <cstdint>

#define MENU_LEN 10

//WARNING
//At time of writing this, I'm prototyping the menu
//with the ncurses library on host machine. Making sure
//this symbol exists so I don't accidently shoot myself
//in the foot by having mismatched knob counts
#ifndef KNOB_COUNT
#define KNOB_COUNT 5
#endif

#define KNOB_RANGE_TO_MS_RANGE(knob_val, ms_range) (knob_val/(UINT16_MAX/ms_range))

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
    LFO2,
    MENU_END
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
    Screen* lcd;
    MenuID selected_menu = MenuID::MENU_END;
    uint16_t knob_readings[KNOB_COUNT];
    int menu_index = 0;
    
    char out[10]; //buffer for printing to LCD
    void increment_index(int inc);
    void navigate(int reading);
    void select();
    void init(Screen* _lcd);
    void update_knob_readings(const uint16_t new_readings[KNOB_COUNT]);
    void update_menu_params(int param_num);

    void print_menu_params();
    void voice_menu_print();
    void pitch_menu_print();
    void oscillator_menu_print();
    void mixer_menu_print();
    void filter_menu_print();
    void filter_env_menu_print();
    void amp_menu_print();
    void amp_env_menu_print();
    void lfo1_menu_print();
    void lfo2_menu_print();
    void print_menu_dummy_params();
};

#endif