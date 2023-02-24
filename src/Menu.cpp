#include "Menu.h"
#include "cstring"
#include "LCDScreen.h"

MenuOption::MenuOption(const char* _name, MenuID _id) {
    strcpy(name, _name);
    id = _id;
}

//Gives menu access to LCD screen
// void Menu::init(LCDScreen* _lcd) {
//     lcd = _lcd;
// }

//When rotary encoder is turned, we're moving options around on the screen
void Menu::navigate(int reading, LCDScreen* lcd) {
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
}

void Menu::select(LCDScreen* lcd) {
    char out[10];
    MenuOption* m = &menu[menu_index];
    lcd->clear();
    lcd->put_cur(0, 0);
    switch(m->id) {
        case MenuID::VOICE: {
            sprintf(out, "OscCnt %d", m->get_p1());
            lcd->send_string(out);
            lcd->put_cur(1,0);
            sprintf(out, "VceAsn %d", m->get_p2());
            lcd->send_string(out);
            lcd->put_cur(2,0);
            sprintf(out, "TrgMod %d", m->get_p3());
            lcd->send_string(out);
            lcd->put_cur(3,0);
            sprintf(out, "UnisDet %d", m->get_p4());
            lcd->send_string(out);
            break;
        }
        case MenuID::PITCH: {
            sprintf(out, "Trnspos %d", m->get_p1());
            lcd->send_string(out);
            lcd->put_cur(1,0);
            sprintf(out, "Tune %d", m->get_p2());
            lcd->send_string(out);
            lcd->put_cur(2,0);
            sprintf(out, "Portamento %d", m->get_p3());
            lcd->send_string(out);
            lcd->put_cur(3,0);
            sprintf(out, "BendRng %d", m->get_p4());
            lcd->send_string(out);
            break;
        }
        case MenuID::OSCILLATORS: {
            sprintf(out, "Osc1 Wave %d", m->get_p1());
            lcd->send_string(out);
            lcd->put_cur(1,0);
            sprintf(out, "Osc2 Wave %d", m->get_p2());
            lcd->send_string(out);
            lcd->put_cur(2,0);
            sprintf(out, "Osc2 Semi %d", m->get_p3());
            lcd->send_string(out);
            lcd->put_cur(3,0);
            sprintf(out, "Osc2 Tune %d", m->get_p4());
            lcd->send_string(out);
            break;
        }
        case MenuID::MIXER: {
            sprintf(out, "Osc1 Lvl %d", m->get_p1());
            lcd->send_string(out);
            lcd->put_cur(1,0);
            sprintf(out, "Osc2 Lvl %d", m->get_p2());
            lcd->send_string(out);
            lcd->put_cur(2,0);
            sprintf(out, "Noise Lvl %d", m->get_p3());
            lcd->send_string(out);
            break;
        }
        case MenuID::FILTER: {
            sprintf(out, "Type %d", m->get_p1());
            lcd->send_string(out);
            lcd->put_cur(1,0);
            sprintf(out, "Cutoff %d", m->get_p2());
            lcd->send_string(out);
            lcd->put_cur(2,0);
            sprintf(out, "Resonance %d", m->get_p3());
            lcd->send_string(out);
            lcd->put_cur(3,0);
            sprintf(out, "FiltEnvInt %d", m->get_p4());
            lcd->send_string(out);
            break;
        }
        case MenuID::FILTER_ENV: {
            sprintf(out, "Attack %d", m->get_p1());
            lcd->send_string(out);
            lcd->put_cur(1,0);
            sprintf(out, "Decay %d", m->get_p2());
            lcd->send_string(out);
            lcd->put_cur(2,0);
            sprintf(out, "Sustain %d", m->get_p3());
            lcd->send_string(out);
            lcd->put_cur(3,0);
            sprintf(out, "Release %d", m->get_p4());
            lcd->send_string(out);
            break;
        }
        case MenuID::AMP: {
            sprintf(out, "Level %d", m->get_p1());
            lcd->send_string(out);
            lcd->put_cur(1,0);
            sprintf(out, "PanPot %d", m->get_p2());
            lcd->send_string(out);
            lcd->put_cur(2,0);
            sprintf(out, "Distortion %d", m->get_p3());
            lcd->send_string(out);
            lcd->put_cur(3,0);
            sprintf(out, "KBD Track %d", m->get_p4());
            lcd->send_string(out);
            break;
        }
        case MenuID::AMP_ENV: {
            sprintf(out, "Attack %d", m->get_p1());
            lcd->send_string(out);
            lcd->put_cur(1,0);
            sprintf(out, "Decay %d", m->get_p2());
            lcd->send_string(out);
            lcd->put_cur(2,0);
            sprintf(out, "Sustain %d", m->get_p3());
            lcd->send_string(out);
            lcd->put_cur(3,0);
            sprintf(out, "Release %d", m->get_p4());
            lcd->send_string(out);
            break;
        }
        case MenuID::LFO1: {
            sprintf(out, "Wave %d", m->get_p1());
            lcd->send_string(out);
            lcd->put_cur(1,0);
            sprintf(out, "Key Sync %d", m->get_p2());
            lcd->send_string(out);
            lcd->put_cur(2,0);
            sprintf(out, "Temp Sync %d", m->get_p3());
            lcd->send_string(out);
            lcd->put_cur(3,0);
            sprintf(out, "Frequency %d", m->get_p4());
            lcd->send_string(out);
            break;
        }
        case MenuID::LFO2: {
            sprintf(out, "Wave %d", m->get_p1());
            lcd->send_string(out);
            lcd->put_cur(1,0);
            sprintf(out, "Key Sync %d", m->get_p2());
            lcd->send_string(out);
            lcd->put_cur(2,0);
            sprintf(out, "Temp Sync %d", m->get_p3());
            lcd->send_string(out);
            lcd->put_cur(3,0);
            sprintf(out, "Frequency %d", m->get_p4());
            lcd->send_string(out);
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