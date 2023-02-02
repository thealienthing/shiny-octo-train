#include "Menu.h"
#include "cstring"
#include <cstdio>
#include "LCDScreen.h"

extern PatchParams patch_params;

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
    char menu_out[20];
    increment_index(reading);
    lcd->clear();
    int index = menu_index;
    for(int i = 0; i < 4; i++) {
        lcd->put_cur(i, 0);
        sprintf(menu_out, "%s", menu[index].name);
        lcd->send_string(menu_out);
        index = (index + 1) % MENU_LEN;
        if(i == 0) {
            sprintf(menu_out, "%s", " <-");
            lcd->send_string(menu_out);
        }
    }
    lcd->put_cur(1, 10);
    selected_menu = MENU_END;
}

void Menu::select() {
    
    selected_menu = menu[menu_index].id;
    lcd->clear();
    lcd->put_cur(0, 0);
    switch(selected_menu) {
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
    print_menu_params();
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

void Menu::print_menu_params() {
    switch(selected_menu) {
        case MenuID::VOICE: {
            print_menu_dummy_params();
            break;
        }
        case MenuID::PITCH: {
            print_menu_dummy_params();
            break;
        }
        case MenuID::OSCILLATORS: {
            print_menu_dummy_params();
            break;
        }
        case MenuID::MIXER: {
            print_menu_dummy_params();
            break;
        }
        case MenuID::FILTER: {
            print_menu_dummy_params();
            break;
        }
        case MenuID::FILTER_ENV: {
            print_menu_dummy_params();
            break;
        }
        case MenuID::AMP: {
            print_menu_dummy_params();
            break;
        }
        case MenuID::AMP_ENV: {
            lcd->put_cur(0,10);
            sprintf(out, "%9d", patch_params.amp_env_attack);
            lcd->send_string(out);
            lcd->put_cur(1,10);
            sprintf(out, "%9d", patch_params.amp_env_decay);
            lcd->send_string(out);
            lcd->put_cur(2,10);
            sprintf(out, "%9.2f", patch_params.amp_env_sustain);
            lcd->send_string(out);
            lcd->put_cur(3,10);
            sprintf(out, "%9d", patch_params.amp_env_release);
            lcd->send_string(out);
            break;
        }
        case MenuID::LFO1: {
            print_menu_dummy_params();
            break;
        }
        case MenuID::LFO2: {
            print_menu_dummy_params();
            break;
        }
    }
}

//This method just checks if any knobs have been moved. If a knob
//has been moved, the value will be sent to a param update method
//which will actually process the data and stored the value
void Menu::update_knob_readings(const uint16_t new_readings[KNOB_COUNT]) {
    bool refresh_params = false;
    for(int i = 0; i < KNOB_COUNT; i++) {
        if(new_readings[i] != knob_readings[i]) {
            knob_readings[i] = new_readings[i];
            update_menu_params(i);
            refresh_params = true;
        }
    }
    if(refresh_params){
        print_menu_params();
    }
}

void Menu::update_menu_params(int param_num) {
    switch(selected_menu) {
        case MenuID::VOICE: {
            
            break;
        }
        case MenuID::PITCH: {
            
            break;
        }
        case MenuID::OSCILLATORS: {
            
            break;
        }
        case MenuID::MIXER: {
            
            break;
        }
        case MenuID::FILTER: {
            
            break;
        }
        case MenuID::FILTER_ENV: {
            
            break;
        }
        case MenuID::AMP: {
            
            break;
        }
        case MenuID::AMP_ENV: {
            if(param_num == 0)
                patch_params.amp_env_attack = knob_readings[param_num]/1000;
            else if(param_num == 1)
                patch_params.amp_env_decay = knob_readings[param_num]/1000;
            else if(param_num == 2)
                patch_params.amp_env_sustain = knob_readings[param_num]/(float)UINT16_MAX;
            else if(param_num == 3)
                patch_params.amp_env_release = knob_readings[param_num]/1000;
            else {

            }
            break;
        }
        case MenuID::LFO1: {
            
            break;
        }
        case MenuID::LFO2: {
            
            break;
        }
    }
}

//This method is used to fill param section of menu with fake data
void Menu::print_menu_dummy_params() {
    lcd->put_cur(0,10);
    sprintf(out, "%9s", "foo");
    lcd->send_string(out);
    lcd->put_cur(1,10);
    sprintf(out, "%9s", "bar");
    lcd->send_string(out);
    lcd->put_cur(2,10);
    sprintf(out, "%9s", "fizz");
    lcd->send_string(out);
    lcd->put_cur(3,10);
    sprintf(out, "%9s", "buzz");
    lcd->send_string(out);
}

void Menu::voice_menu_print() {
    
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
    
    sprintf(out, "Osc1Wave ");
    lcd->send_string(out);
    lcd->put_cur(1,0);
    sprintf(out, "Osc2Wave");
    lcd->send_string(out);
    lcd->put_cur(2,0);
    sprintf(out, "Osc2Semi");
    lcd->send_string(out);
    lcd->put_cur(3,0);
    sprintf(out, "Osc2Tun");
    lcd->send_string(out);
}
void Menu::mixer_menu_print() {
    
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
    
    sprintf(out, "Type ");
    lcd->send_string(out);
    lcd->put_cur(1,0);
    sprintf(out, "Cutoff");
    lcd->send_string(out);
    lcd->put_cur(2,0);
    sprintf(out, "Resonance");
    lcd->send_string(out);
    lcd->put_cur(3,0);
    sprintf(out, "EnvInt");
    lcd->send_string(out);
}
void Menu::filter_env_menu_print() {
    
    sprintf(out, "Attack");
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
    
    sprintf(out, "Level ");
    lcd->send_string(out);
    lcd->put_cur(1,0);
    sprintf(out, "PanPot");
    lcd->send_string(out);
    lcd->put_cur(2,0);
    sprintf(out, "Distort");
    lcd->send_string(out);
    lcd->put_cur(3,0);
    sprintf(out, "KBD Track");
    lcd->send_string(out);
}
void Menu::amp_env_menu_print() {
    
    sprintf(out, "Attack");
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