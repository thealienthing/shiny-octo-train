/*
 * Credit is given to the great folks at controllerstech.com who generated/provided this sample code.
 * These files (i2c-lcd.c and i2c-lcd.h) can be found at https://controllerstech.com/i2c-lcd-in-stm32/
 */

/** Put this in the src folder **/
#include "daisy.h"
#include "daisy_seed.h"
#include <stdint.h>
#include "LCDScreen.h"

using namespace daisy;

//extern I2C_HandleTypeDef hi2c1;  // change your handler here accordingly
extern DaisySeed hw;

#define SLAVE_ADDRESS_LCD 0x27 // change this according to ur setup

LCDScreen::LCDScreen(I2CHandle* i2c_handle) {
	i2c = i2c_handle;
}

daisy::I2CHandle::Result LCDScreen::send_cmd (char cmd)
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

daisy::I2CHandle::Result LCDScreen::send_data (char data)
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

void LCDScreen::clear ()
{
	send_cmd (0x01);
	hw.DelayMs(2);
}

void LCDScreen::put_cur(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x00;
            break;
        case 1:
            col |= 0x40;
            break;
		case 2:
            col |= 0x14;
            break;
		case 3:
            col |= 0x54;
            break;
    }
    send_cmd (col);
}


void LCDScreen::init()
{
	// 4 bit initialisation
	hw.DelayMs(50);  // wait for >40ms
	send_cmd(0x30);
	hw.DelayMs(5);  // wait for >4.1ms
	send_cmd(0x30);
	hw.DelayMs(1);  // wait for >100us
	send_cmd(0x30);
	hw.DelayMs(10);
	send_cmd(0x20);  // 4bit mode
	hw.DelayMs(10);

  // dislay initialisation
	send_cmd(0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	hw.DelayMs(1);
	send_cmd(0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	hw.DelayMs(1);
	send_cmd(0x01);  // clear display
	hw.DelayMs(1);
	hw.DelayMs(1);
	send_cmd(0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	hw.DelayMs(1);
	send_cmd(0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

void LCDScreen::send_string (char *str)
{
	while (*str) send_data (*str++);
}
