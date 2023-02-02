#include <stdio.h>
#include "CursesScreen.h"
#include "../../src/Menu.h"
#include "../../src/PatchParams.h"

Screen *screen;
Menu menu;
PatchParams patch_params;
uint16_t knobs[KNOB_COUNT] = {0, 0, 0, 0, 0};

#define INCR 1000

void encoder_read(char c) {
    if(c=='A') {
        menu.navigate(-1);
    }
    else if(c=='B') {
        menu.navigate(1);
    }
    else if(c=='C' || c=='D') {
        menu.select();
    }
}

uint16_t incr(uint16_t knob) {
    if(knob + INCR > __UINT16_MAX__) {
        return __UINT16_MAX__;
    }
    else {
        return knob + INCR;
    }
}

uint16_t decr(uint16_t knob) {
    if((int)knob - INCR < 0) {
        return 0;
    }
    else {
        return knob - INCR;
    }
}

void knob_read(char c) {
    switch(c){
        case '1': {
            knobs[0] = incr(knobs[0]);
            break;
        }
        case '!': {
            knobs[0] = decr(knobs[0]);
            //printf("knob1 ccw\n");
            break;
        }
        case '2': {
            knobs[1] = incr(knobs[1]);
            //printf("knob2 cw\n");
            break;
        }
        case '@': {
            knobs[1] = decr(knobs[1]);
            //printf("knob2 ccw\n");
            break;
        }
        case '3': {
            knobs[2] = incr(knobs[2]);
            //printf("knob3 cw\n");
            break;
        }
        case '#': {
            knobs[2] = decr(knobs[2]);
            //printf("knob3 ccw\n");
            break;
        }
        case '4': {
            knobs[3] = incr(knobs[3]);
            //printf("knob4 cw\n");
            break;
        }
        case '$': {
            knobs[3] = decr(knobs[3]);
            //printf("knob4 ccw\n");
            break;
        }
        default: {
            return;
        }
    }
    menu.update_knob_readings(knobs);
    menu.print_menu_params();
}

bool process(Menu& menu) {
    char c = getch();
    encoder_read(c);
    knob_read(c);

    if(c == 'q') {
        return false;
    }
    else {
        return true;
    }
    
    
}

int main() {
    screen = new CursesScreen();
    screen->init();
    menu.init(screen);
    
    while(process(menu)) {
        //do nothing
    }
    echo();
    endwin();
    refresh();
    return 0;
}