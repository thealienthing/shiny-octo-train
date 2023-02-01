#include "Menu.h"
#include "cstring"
#include <cstdio>
#include "LCDScreen.h"

MenuOption::MenuOption(const char* _name, MenuID _id) {
    strcpy(name, _name);
    id = _id;
}

//Gives menu access to LCD screen
void Menu::init(Screen* _lcd) {
    lcd = _lcd;
}

//When rotary encoder is turned, we're moving options around on the screen
void Menu::navigate(int reading) {
    char out[20];
    increment_index(reading);
    lcd->clear();
    int index = menu_index;
    for(int i = 0; i < 4; i++) {
        lcd->put_cur(i, 0);
        sprintf(out, "%s", menu[index].name);
        lcd->send_string(out);
        index = (index + 1) % MENU_LEN;
        if(i == 0) {
            lcd->send_string(" <-");
        }
    }
    lcd->put_cur(1, 10);
    selected_id = MENU_END;
}

void Menu::select() {
    char out[20];
    selected_id = menu[menu_index].id;
    lcd->clear();
    lcd->put_cur(0, 0);
    switch(selected_id) {
        case MenuID::VOICE: {
            voice_menu_print();
            break;
        }
        case MenuID::PITCH: {
            pitch_menu_print();
            break;
        }
        case MenuID::OSCILLATORS: {
            oscillator_menu_print();
            break;
        }
        case MenuID::MIXER: {
            mixer_menu_print();
            break;
        }
        case MenuID::FILTER: {
            filter_menu_print();
            break;
        }
        case MenuID::FILTER_ENV: {
            filter_env_menu_print();
            break;
        }
        case MenuID::AMP: {
            amp_menu_print();
            break;
        }
        case MenuID::AMP_ENV: {
            amp_env_menu_print();
            break;
        }
        case MenuID::LFO1: {
            lfo1_menu_print();
            break;
        }
        case MenuID::LFO2: {
            lfo2_menu_print();
            break;
        }
    }
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

void Menu::voice_menu_print() {
    char out[20];
    sprintf(out, "OscCnt");
    lcd->send_string(out);
    lcd->put_cur(1,0);
    sprintf(out, "VceAsn");
    lcd->send_string(out);
    lcd->put_cur(2,0);
    sprintf(out, "TrgMod");
    lcd->send_string(out);
    lcd->put_cur(3,0);
    sprintf(out, "UnisDet");
    lcd->send_string(out);
}

void Menu::pitch_menu_print() {
    char out[20];
    sprintf(out, "Trnspos ");
    lcd->send_string(out);
    lcd->put_cur(1,0);
    sprintf(out, "Tune");
    lcd->send_string(out);
    lcd->put_cur(2,0);
    sprintf(out, "Porta");
    lcd->send_string(out);
    lcd->put_cur(3,0);
    sprintf(out, "BendRng");
    lcd->send_string(out);
}

void Menu::oscillator_menu_print() {
    char out[20];
    sprintf(out, "Osc1 Wave ");
    lcd->send_string(out);
    lcd->put_cur(1,0);
    sprintf(out, "Osc2 Wave");
    lcd->send_string(out);
    lcd->put_cur(2,0);
    sprintf(out, "Osc2Semi");
    lcd->send_string(out);
    lcd->put_cur(3,0);
    sprintf(out, "Osc2Tun");
    lcd->send_string(out);
}
void Menu::mixer_menu_print() {
    char out[20];
    sprintf(out, "Osc1 Lvl ");
    lcd->send_string(out);
    lcd->put_cur(1,0);
    sprintf(out, "Osc2 Lvl");
    lcd->send_string(out);
    lcd->put_cur(2,0);
    sprintf(out, "Noise Lvl");
    lcd->send_string(out);

}
void Menu::filter_menu_print() {
    char out[20];
    sprintf(out, "Type ");
    lcd->send_string(out);
    lcd->put_cur(1,0);
    sprintf(out, "Cutoff");
    lcd->send_string(out);
    lcd->put_cur(2,0);
    sprintf(out, "Resonance");
    lcd->send_string(out);
    lcd->put_cur(3,0);
    sprintf(out, "FiltEnvInt");
    lcd->send_string(out);
}
void Menu::filter_env_menu_print() {
    char out[20];
    sprintf(out, "Attack ");
    lcd->send_string(out);
    lcd->put_cur(1,0);
    sprintf(out, "Decay");
    lcd->send_string(out);
    lcd->put_cur(2,0);
    sprintf(out, "Sustain");
    lcd->send_string(out);
    lcd->put_cur(3,0);
    sprintf(out, "Release");
    lcd->send_string(out);

}
void Menu::amp_menu_print() {
    char out[20];
    sprintf(out, "Level ");
    lcd->send_string(out);
    lcd->put_cur(1,0);
    sprintf(out, "PanPot");
    lcd->send_string(out);
    lcd->put_cur(2,0);
    sprintf(out, "Distortion");
    lcd->send_string(out);
    lcd->put_cur(3,0);
    sprintf(out, "KBD Track");
    lcd->send_string(out);
}
void Menu::amp_env_menu_print() {
    char out[20];
    sprintf(out, "Attack ");
    lcd->send_string(out);
    lcd->put_cur(1,0);
    sprintf(out, "Decay");
    lcd->send_string(out);
    lcd->put_cur(2,0);
    sprintf(out, "Sustain");
    lcd->send_string(out);
    lcd->put_cur(3,0);
    sprintf(out, "Release");
    lcd->send_string(out);
}
void Menu::lfo1_menu_print() {
    char out[20];
    sprintf(out, "Wave ");
    lcd->send_string(out);
    lcd->put_cur(1,0);
    sprintf(out, "Key Sync");
    lcd->send_string(out);
    lcd->put_cur(2,0);
    sprintf(out, "Temp Sync");
    lcd->send_string(out);
    lcd->put_cur(3,0);
    sprintf(out, "Frequency");
    lcd->send_string(out);
}
void Menu::lfo2_menu_print() {
    char out[20];
    sprintf(out, "Wave ");
    lcd->send_string(out);
    lcd->put_cur(1,0);
    sprintf(out, "Key Sync");
    lcd->send_string(out);
    lcd->put_cur(2,0);
    sprintf(out, "Temp Sync");
    lcd->send_string(out);
    lcd->put_cur(3,0);
    sprintf(out, "Frequency");
    lcd->send_string(out);
}