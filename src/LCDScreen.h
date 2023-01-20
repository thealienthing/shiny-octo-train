/*
 * Credit is given to the great folks at controllerstech.com who generated/provided this sample code.
 * These files (i2c-lcd.c and i2c-lcd.h) can be found at https://controllerstech.com/i2c-lcd-in-stm32/
 */

#ifndef LCDSCREEN_H
#define LCDSCREEN_H

#include "daisy.h"
#include "hardware.h"

using namespace daisy;

class LCDScreen {
public:
    LCDScreen(I2CHandle* i2c_handle);
    void init();   // initialize lcd
    void send_string (char *str);  // send string to the lcd
    void put_cur(int row, int col);  // put cursor at the entered position row (0 or 1), col (0-15);
    void clear();

private:
    I2CHandle* i2c;
    daisy::I2CHandle::Result send_cmd (char cmd);  // send command to the lcd
    daisy::I2CHandle::Result send_data (char data);  // send data to the lcd
};

#endif
