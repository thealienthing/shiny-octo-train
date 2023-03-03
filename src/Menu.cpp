#include "Menu.h"
#include "cstring"
#include <cstdio>
#include "LCDScreen.h"

extern PatchParams patch_params;
char WaveFormStrings[(int)WaveFormEnd][10] = {"Sin", "Tri", "Saw", "Square", "Noise"};
char FilterTypeStrings[(int)FilterTypeEnd][10] = {"-24Low", "-12Low", "-12Band", "-12High"};

MenuOption::MenuOption(const char* _name, MenuID _id) {
    strcpy(name, _name);
    id = _id;
}

void Menu::init(LCDScreen* l, PatchParams* p) {
    lcd = l;
    patch_params = p;
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
    char out[10];
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
            WaveForm wave_index[] = {WaveForm::Sin, WaveForm::Tri, WaveForm::Saw, WaveForm::Square, WaveForm::WhiteNoise};
            lcd->put_cur(0,10);
            sprintf(out, "%9s", WaveFormStrings[wave_index[patch_params->osc1_waveform]]);
            lcd->send_string(out);
            lcd->put_cur(1,10);
            sprintf(out, "%9s", WaveFormStrings[wave_index[patch_params->osc2_waveform]]);
            lcd->send_string(out);
            lcd->put_cur(2,10);
            sprintf(out, "%9d", patch_params->osc2_semitone);
            lcd->send_string(out);
            lcd->put_cur(3,10);
            sprintf(out, "%9d", patch_params->osc2_tune);
            lcd->send_string(out);
            break;
        }
        case MenuID::MIXER: {
            lcd->put_cur(0,10);
            sprintf(out, "%9.2f", patch_params->oscillator1_level);
            lcd->send_string(out);
            lcd->put_cur(1,10);
            sprintf(out, "%9.2f", patch_params->oscillator2_level);
            lcd->send_string(out);
            lcd->put_cur(2,10);
            sprintf(out, "%9.2f", patch_params->noise_level);
            lcd->send_string(out);
            break;
        }
        case MenuID::FILTER: {
            FilterType filter_index[] = {FilterType::LowPass24db, FilterType::LowPass12db, FilterType::BandPass12db, FilterType::HighPass12db};
            lcd->put_cur(0,10);
            sprintf(out, "%9s", FilterTypeStrings[filter_index[patch_params->filter_type]]);
            lcd->send_string(out);
            lcd->put_cur(1,10);
            sprintf(out, "%9d", patch_params->filter_cutoff);
            lcd->send_string(out);
            lcd->put_cur(2,10);
            sprintf(out, "%9.2f", patch_params->filter_resonance);
            lcd->send_string(out);
            lcd->put_cur(3,10);
            sprintf(out, "%9.2f", patch_params->filter_env_intensity);
            lcd->send_string(out);
            
            break;
        }
        case MenuID::FILTER_ENV: {
            lcd->put_cur(0,10);
            sprintf(out, "%9d", patch_params->filter_env_attack);
            lcd->send_string(out);
            lcd->put_cur(1,10);
            sprintf(out, "%9d", patch_params->filter_env_decay);
            lcd->send_string(out);
            lcd->put_cur(2,10);
            sprintf(out, "%9.2f", patch_params->filter_env_sustain);
            lcd->send_string(out);
            lcd->put_cur(3,10);
            sprintf(out, "%9d", patch_params->filter_env_release);
            lcd->send_string(out);
            break;
        }
        case MenuID::AMP: {
            print_menu_dummy_params();
            break;
        }
        case MenuID::AMP_ENV: {
            lcd->put_cur(0,10);
            sprintf(out, "%9d", patch_params->amp_env_attack);
            lcd->send_string(out);
            lcd->put_cur(1,10);
            sprintf(out, "%9d", patch_params->amp_env_decay);
            lcd->send_string(out);
            lcd->put_cur(2,10);
            sprintf(out, "%9.2f", patch_params->amp_env_sustain);
            lcd->send_string(out);
            lcd->put_cur(3,10);
            sprintf(out, "%9d", patch_params->amp_env_release);
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
        if( abs((int32_t)new_readings[i]-(int32_t)knob_readings[i]) > 5){
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
            WaveForm wave_index[] = {WaveForm::Sin, WaveForm::Tri, WaveForm::Saw, WaveForm::Square, WaveForm::WhiteNoise};
            
            if(param_num == 0){
                patch_params->osc1_waveform = wave_index[KNOB_RANGE_TO_INT(knob_readings[param_num],(int)WaveForm::WhiteNoise)];
            }
            else if(param_num == 1) {
                patch_params->osc2_waveform = wave_index[KNOB_RANGE_TO_INT(knob_readings[param_num],(int)WaveForm::WhiteNoise)];
            }
            else if(param_num == 2) {
                patch_params->osc2_semitone = (knob_readings[param_num]/(UINT16_MAX/48))-24;
            }
            else if(param_num == 3) {
                patch_params->osc2_tune = (knob_readings[param_num]/(UINT16_MAX/100))-50;
            }
            else {

            }
            break;
        }
        case MenuID::MIXER: {
            if(param_num == 0){
                patch_params->oscillator1_level = knob_readings[param_num]/(float)UINT16_MAX;
            }
            else if(param_num == 1){
                //Same thing here for decay time. Max decay time 5000ms
                patch_params->oscillator2_level = knob_readings[param_num]/(float)UINT16_MAX;
            }
            else if(param_num == 2){
                //Volume range 0-1.0
                patch_params->noise_level = knob_readings[param_num]/(float)UINT16_MAX;
            }
            else {

            }
            break;
        }
        case MenuID::FILTER: {
            FilterType filter_index[] = {FilterType::LowPass24db, FilterType::LowPass12db, FilterType::BandPass12db, FilterType::HighPass12db};
            if(param_num == 0){
                patch_params->filter_type = filter_index[KNOB_RANGE_TO_INT(knob_readings[param_num],(int)FilterType::HighPass12db)];
            }
            else if(param_num == 1) {
                patch_params->filter_cutoff = knob_readings[param_num]/(float)UINT16_MAX*100;
            }
            else if(param_num == 2) {
                patch_params->filter_resonance = knob_readings[param_num]/(float)UINT16_MAX*2.0;
            }
            else if(param_num == 3) {
                patch_params->filter_env_intensity = knob_readings[param_num]/(float)UINT16_MAX;
            }
            break;
        }
        case MenuID::FILTER_ENV: {
            if(param_num == 0)
                //Divide uint16_t max by 13 to get an approximate increment for a range between 0-5000ms
                //Add 10 to baseline to remove potential ugly clipping of volume jump at min attack
                patch_params->filter_env_attack = KNOB_RANGE_TO_MS_RANGE(knob_readings[param_num], 5000)+10;
            else if(param_num == 1)
                //Same thing here for decay time. Max decay time 5000ms
                patch_params->filter_env_decay = KNOB_RANGE_TO_MS_RANGE(knob_readings[param_num], 5000)+10;
            else if(param_num == 2)
                //Volume range 0-1.0
                patch_params->filter_env_sustain = knob_readings[param_num]/(float)UINT16_MAX;
            else if(param_num == 3)
                //Same thing here as with attack and decay. Max release 5000ms
                patch_params->filter_env_release = KNOB_RANGE_TO_MS_RANGE(knob_readings[param_num], 5000)+10;
            else {

            }
            break;
        }
        case MenuID::AMP: {
            
            break;
        }
        case MenuID::AMP_ENV: {
            if(param_num == 0)
                //Divide uint16_t max by 13 to get an approximate increment for a range between 0-5000ms
                //Add 10 to baseline to remove potential ugly clipping of volume jump at min attack
                patch_params->amp_env_attack = KNOB_RANGE_TO_MS_RANGE(knob_readings[param_num], 5000)+10;
            else if(param_num == 1)
                //Same thing here for decay time. Max decay time 5000ms
                patch_params->amp_env_decay = KNOB_RANGE_TO_MS_RANGE(knob_readings[param_num], 5000)+10;
            else if(param_num == 2)
                //Volume range 0-1.0
                patch_params->amp_env_sustain = knob_readings[param_num]/(float)UINT16_MAX;
            else if(param_num == 3)
                //Same thing here as with attack and decay. Max release 5000ms
                patch_params->amp_env_release = KNOB_RANGE_TO_MS_RANGE(knob_readings[param_num], 5000)+10;
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