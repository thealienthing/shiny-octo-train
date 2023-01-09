/*
 * Credit is given to the great folks at controllerstech.com who generated/provided this sample code.
 * These files (i2c-lcd.c and i2c-lcd.h) can be found at https://controllerstech.com/i2c-lcd-in-stm32/
 */

/** Put this in the src folder **/
#include "daisy.h"
#include "daisy_seed.h"
#include <stdint.h>
#include "i2c-lcd.h"

using namespace daisy;

//extern I2C_HandleTypeDef hi2c1;  // change your handler here accordingly
extern I2CHandle i2c;

#define SLAVE_ADDRESS_LCD 0x27 // change this according to ur setup

I2CHandle::Result lcd_send_cmd ( I2CHandle* i2c, char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	return i2c->TransmitBlocking(SLAVE_ADDRESS_LCD, (uint8_t *) data_t, 4, 100);
	//HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

I2CHandle::Result lcd_send_data ( I2CHandle* i2c, char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0D;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0
	return i2c->TransmitBlocking(SLAVE_ADDRESS_LCD, (uint8_t *) data_t, 4, 100);
	//HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_clear ( I2CHandle* i2c)
{
	lcd_send_cmd (i2c, 0x80);
	for (int i=0; i<70; i++)
	{
		lcd_send_data ( i2c, ' ');
	}
}

void lcd_put_cur( I2CHandle* i2c, int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }

    lcd_send_cmd ( i2c, col);
}


void lcd_init ( I2CHandle* i2c)
{
	// 4 bit initialisation
	HAL_Delay(50);  // wait for >40ms
	lcd_send_cmd ( i2c, 0x30);
	HAL_Delay(5);  // wait for >4.1ms
	lcd_send_cmd ( i2c, 0x30);
	HAL_Delay(1);  // wait for >100us
	lcd_send_cmd ( i2c, 0x30);
	HAL_Delay(10);
	lcd_send_cmd ( i2c, 0x20);  // 4bit mode
	HAL_Delay(10);

  // dislay initialisation
	lcd_send_cmd ( i2c, 0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	lcd_send_cmd ( i2c, 0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(1);
	lcd_send_cmd ( i2c, 0x01);  // clear display
	HAL_Delay(1);
	HAL_Delay(1);
	lcd_send_cmd ( i2c, 0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	lcd_send_cmd ( i2c, 0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

void lcd_send_string ( I2CHandle* i2c, char *str)
{
	while (*str) lcd_send_data ( i2c, *str++);
}
