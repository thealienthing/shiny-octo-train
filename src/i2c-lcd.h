/*
 * Credit is given to the great folks at controllerstech.com who generated/provided this sample code.
 * These files (i2c-lcd.c and i2c-lcd.h) can be found at https://controllerstech.com/i2c-lcd-in-stm32/
 */

#ifndef I2C_LCD_H
#define I2C_LCD_H

#include "daisy.h"
#include "hardware.h"

using namespace daisy;

void lcd_init ( I2CHandle* i2c);   // initialize lcd

daisy::I2CHandle::Result lcd_send_cmd ( I2CHandle* i2c, char cmd);  // send command to the lcd

daisy::I2CHandle::Result lcd_send_data ( I2CHandle* i2c, char data);  // send data to the lcd

void lcd_send_string ( I2CHandle* i2c, char *str);  // send string to the lcd

void lcd_put_cur( I2CHandle* i2c, int row, int col);  // put cursor at the entered position row (0 or 1), col (0-15);

void lcd_clear ( I2CHandle* i2c);

#endif
