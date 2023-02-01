/*
 * Credit is given to the great folks at controllerstech.com who generated/provided this sample code.
 * These files (i2c-lcd.c and i2c-lcd.h) can be found at https://controllerstech.com/i2c-lcd-in-stm32/
 */

#ifndef LCDSCREEN_H
#define LCDSCREEN_H

class Screen {
public:
    virtual void init() = 0;
    virtual void send_string (char *str) = 0;
    virtual void put_cur(int row, int col) = 0;
    virtual void cursor_setup(bool cur_on, bool blink_on) = 0;
    virtual void clear() = 0;
};

#ifdef MASTERS_CAPSTONE
#include "daisy.h"
#include "hardware.h"

using namespace daisy;

#define CURSOR_ON       0x02
#define CURSOR_OFF      0x00
#define CURSOR_BLINK    0x01
#define CURSOR_NOBLINK  0x00
#define ROW1            0x80
#define ROW2            0xC0
#define ROW3            0x94
#define ROW4            0xD4

class LCDScreen : public Screen {
public:
    LCDScreen(I2CHandle* i2c_handle);
    void init() override;   // initialize lcd
    void send_string (char *str) override;  // send string to the lcd
    void put_cur(int row, int col) override;  // put cursor at the entered position row (0 or 1), col (0-15);
    void cursor_setup(bool cur_on, bool blink_on) override;
    void clear() override;

    bool cursor_on = false;
    bool cursor_blink = false;

private:
    daisy::I2CHandle::Result send_cmd (char cmd);  // send command to the lcd
    daisy::I2CHandle::Result send_data (char data);  // send data to the lcd
    I2CHandle* i2c;
};
#endif

#endif
