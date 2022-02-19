/*
* Project Name: SSD1305_OLED_RPI
* Author: Gavin Lyons.
* URL: https://github.com/gavinlyonsrepo/SSD1305_OLED_RPI
*/

#include "SSD1305_OLED.h"
#include <stdbool.h>

SSD1305 ::SSD1305(int16_t oledwidth, int16_t oledheight) : SSD1305_graphics(oledwidth, oledheight)
{
	_OLED_HEIGHT = oledheight;
	_OLED_WIDTH = oledwidth;
	_OLED_PAGE_NUM = (_OLED_HEIGHT / 8);
	bufferWidth = _OLED_WIDTH;
	bufferHeight = _OLED_HEIGHT;
}

// Desc: begin Method initialise OLED
void SSD1305::OLEDbegin()
{
	OLED_I2C_ON();
	OLEDinit();
	OLED_I2C_OFF();
}

void SSD1305::OLED_I2C_ON()
{
	// Start I2C operations. Forces RPi I2C pins P1-03 (SDA) and P1-05 (SCL)
	// to alternate function ALT0, which enables those pins for I2C interface.
	if (!bcm2835_i2c_begin())
	{
		printf("Error: Cannot start I2C\n");
		return;
	}
	bcm2835_i2c_setSlaveAddress(SSD1305_ADDR); //i2c address
	bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_626);

	//(1) BCM2835_I2C_CLOCK_DIVIDER_626  ：622 = 2.504us = 399.3610 kHz
	//Clock divided is based on nominal base clock rate of 250MHz

	// (2) or use set_baudrate instead of clockdivder
	//bcm2835_i2c_set_baudrate(100000); //100k baudrate
}

// Desc: End I2C operations.
// I2C pins P1-03 (SDA) and P1-05 (SCL)
// are returned to their default INPUT behaviour.
void SSD1305::OLED_I2C_OFF(void)
{
	bcm2835_i2c_end();
}

// Call when powering down
void SSD1305::OLEDPowerDown(void)
{
	OLEDEnable(0);
	bcm2835_delay(100);
}

// Desc: Called from OLEDbegin carries out Power on sequence and register init
void SSD1305::OLEDinit()
{

	bcm2835_delay(SSD1305_INITDELAY);


	SSD1305_command( SSD1305_DISPLAY_OFF);
	SSD1305_command( SSD1305_SET_DISPLAY_CLOCK_DIV_RATIO);
	SSD1305_command( 0x80);
	SSD1305_command( SSD1305_SET_MULTIPLEX_RATIO );
	SSD1305_command( _OLED_HEIGHT - 1 );
	SSD1305_command( SSD1305_SET_DISPLAY_OFFSET );
	SSD1305_command(0x00);
	SSD1305_command( SSD1305_SET_START_LINE|0x00);
	SSD1305_command( SSD1305_CHARGE_PUMP );
	SSD1305_command(0x14);
	SSD1305_command( SSD1305_MEMORY_ADDR_MODE );
	SSD1305_command(0x00);  //Horizontal Addressing Mode is Used
	SSD1305_command( SSD1305_SET_SEGMENT_REMAP| 0x01);
	SSD1305_command( SSD1305_COM_SCAN_DIR_DEC );
	
	//!EDIT added init commands with my own commands
	// from https://github.com/rm-hull/luma.oled/issues/309

	SSD1305_command(0xAE);
	SSD1305_command(0x04);
	SSD1305_command(0x10);
	SSD1305_command(0x40);
	SSD1305_command(0x81);
	SSD1305_command(0x80);
	SSD1305_command(0xA1);
	SSD1305_command(0xA6);
	SSD1305_command(0xA8);
	SSD1305_command(0x1F);
	SSD1305_command(0xC8);
	SSD1305_command(0xD3);
	SSD1305_command(0x00);
	SSD1305_command(0xD5);
	SSD1305_command(0xF0); //flackert, aber kein piepen wenn auskommentiert
	SSD1305_command(0xd8);
	SSD1305_command(0x05); // bild kaputt wenn auskommentiert
	SSD1305_command(0xD9); // same
	SSD1305_command(0xC2);
	SSD1305_command(0xDA);
	SSD1305_command(0x12);
	SSD1305_command(0xDB);
	SSD1305_command(0x08);
	SSD1305_command(0xAF);

	// from the Adafruit Python library
	/*SSD1305_command(0xAE | 0x00);
		SSD1305_command(0xD5);
		SSD1305_command(0x80);
		SSD1305_command(0xA0 | 0x01);
		SSD1305_command(32-1);
		SSD1305_command(0xD3);
		SSD1305_command(0x00);
		SSD1305_command(0x8E);
		SSD1305_command(0xD8);
		SSD1305_command(0x05);
		SSD1305_command(0x20);
		SSD1305_command(0x00);
		SSD1305_command(0x40 | 0x00);
		SSD1305_command(0x2E);
		SSD1305_command(0xDA);
		SSD1305_command(0x12);
		SSD1305_command(0x91);
		SSD1305_command(0x3F);
		SSD1305_command(0x3F);
		SSD1305_command(0x3F);
		SSD1305_command(0x3F);
		SSD1305_command(0x81);
		SSD1305_command(0xFF);
		SSD1305_command(0xD9);
		SSD1305_command(0xD2);
		SSD1305_command(0xDB);
		SSD1305_command(0x34);
		SSD1305_command(0xA6);
		SSD1305_command(0xA4);
		SSD1305_command(0x8D);
		SSD1305_command(0x10 );
		SSD1305_command(0xAE | 0x01);*/

	switch (_OLED_HEIGHT)
	{
	case 64: // NOTE: not tested, lacking part. most likely not gonna work
		SSD1305_command(SSD1305_SET_COM_PINS);
		SSD1305_command(0x12);
		SSD1305_command(SSD1305_SET_CONTRAST_CONTROL);
		SSD1305_command(0xCF);
		break;
	case 32:

		//!EDIT commented it out, because it made it to display only every second row
		// from https://github.com/rm-hull/luma.oled/issues/309
		// SSD1305_command(SSD1305_SET_COM_PINS);
		SSD1305_command(0x02);
		SSD1305_command(SSD1305_SET_CONTRAST_CONTROL);
		SSD1305_command(0x8F);

		break;
	case 16: // NOTE: not tested, lacking part. most likely not gonna work
		SSD1305_command(SSD1305_SET_COM_PINS);
		SSD1305_command(0x2); // ?
		SSD1305_command(SSD1305_SET_CONTRAST_CONTROL);
		SSD1305_command(0xAF);
		break;
	}

	SSD1305_command(SSD1305_SET_PRECHARGE_PERIOD);
	SSD1305_command(0xF1);
	SSD1305_command(SSD1305_SET_VCOM_DESELECT);
	SSD1305_command(0x40);
	SSD1305_command(SSD1305_DISPLAY_ALL_ON_RESUME);
	SSD1305_command(SSD1305_NORMAL_DISPLAY);
	SSD1305_command(SSD1305_DEACTIVATE_SCROLL);
	SSD1305_command(SSD1305_DISPLAY_ON);

	bcm2835_delay(SSD1305_INITDELAY);
}

// Desc: Turns On Display
// Param1: bits,  1  on , 0 off
void SSD1305::OLEDEnable(uint8_t bits)
{
	OLED_I2C_ON();
	bits ? SSD1305_command(SSD1305_DISPLAY_ON) : SSD1305_command(SSD1305_DISPLAY_OFF);
	OLED_I2C_OFF();
}

// Desc: Adjusts contrast
// Param1: Contrast 0x00 to 0xFF , default 0x80
void SSD1305::OLEDContrast(uint8_t contrast)
{
	OLED_I2C_ON();
	SSD1305_command(SSD1305_SET_CONTRAST_CONTROL);
	SSD1305_command(contrast);
	OLED_I2C_OFF();
}

// Desc: invert the display
// Param1: true invert , false normal
void SSD1305::OLEDInvert(bool value)
{
	OLED_I2C_ON();
	value ? SSD1305_command(SSD1305_INVERT_DISPLAY) : SSD1305_command(SSD1305_NORMAL_DISPLAY);
	OLED_I2C_OFF();
}

// Desc: Fill the screen NOT the buffer with a datapattern
// Param1: datapattern can be set to zero to clear screen (not buffer) range 0x00 to 0ff
// Param2: optional delay in milliseconds can be set to zero normally.
void SSD1305::OLEDFillScreen(uint8_t dataPattern, uint8_t delay)
{
	OLED_I2C_ON();
	for (uint8_t row = 0; row < _OLED_PAGE_NUM; row++)
	{
		SSD1305_command(0xB0 | row);
		SSD1305_command(SSD1305_SET_LOWER_COLUMN);
		SSD1305_command(SSD1305_SET_HIGHER_COLUMN);
		for (uint8_t col = 0; col < _OLED_WIDTH; col++)
		{
			SSD1305_data(dataPattern);
			bcm2835_delay(delay);
		}
	}
	OLED_I2C_OFF();
}

// Desc: Fill the chosen page(1-8)  with a datapattern
// Param1: datapattern can be set to 0 to FF (not buffer)
// Param2: optional delay in milliseconds can be set to zero normally.
void SSD1305::OLEDFillPage(uint8_t page_num, uint8_t dataPattern, uint8_t mydelay)
{
	OLED_I2C_ON();
	uint8_t Result = 0xB0 | page_num;
	SSD1305_command(Result);
	SSD1305_command(SSD1305_SET_LOWER_COLUMN);
	SSD1305_command(SSD1305_SET_HIGHER_COLUMN);
	uint8_t numofbytes = _OLED_WIDTH;
	for (uint8_t i = 0; i < numofbytes; i++)
	{
		SSD1305_data(dataPattern);
		bcm2835_delay(mydelay);
	}
	OLED_I2C_OFF();
}

//Desc: Draw a bitmap  to the screen
//Param1: x offset 0-128
//Param2: y offset 0-64
//Param3: width 0-128
//Param4 height 0-64
//Param5: pointer to bitmap data
//Param6: Invert color
void SSD1305::OLEDBitmap(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t *data, bool invert)
{

	int16_t byteWidth = (w + 7) / 8;
	uint8_t byte = 0;
	uint8_t color, bgcolor;
	if (invert == false)
	{
		color = WHITE;
		bgcolor = BLACK;
	}
	else
	{
		color = BLACK;
		bgcolor = WHITE;
	}

	for (int16_t j = 0; j < h; j++, y++)
	{
		for (int16_t i = 0; i < w; i++)
		{
			if (i & 7)
				byte <<= 1;
			else
				byte = data[j * byteWidth + i / 8];

			drawPixel(x + i, y, (byte & 0x80) ? color : bgcolor);
		}
	}
}

// Desc Writes a byte to I2C address,command or reg
// used internally
void SSD1305::I2C_Write_Byte(uint8_t value, uint8_t Cmd)
{
	char buf[2] = {Cmd, value};
	int ref = bcm2835_i2c_write(buf, 2);

	while (ref != 0)
	{
		ref = bcm2835_i2c_write(buf, 2);
		if (ref == 0)
			break;
	}
}

//Desc: updates the buffer i.e. writes it to the screen
void SSD1305::OLEDupdate()
{
	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t w = this->bufferWidth;
	uint8_t h = this->bufferHeight;
	OLEDBuffer(x, y, w, h, (uint8_t *)this->buffer);
}

//Desc: clears the buffer memory i.e. does NOT write to the screen
void SSD1305::OLEDclearBuffer()
{

	memset(this->buffer, 0x00, (this->bufferWidth * (this->bufferHeight / 8)));
}

//Desc: Draw a bitmap to the screen
//Param1: x offset 0-128
//Param2: y offset 0-64
//Param3: width 0-128
//Param4 height 0-64
//Param5 the buffer data
//Note: Called by OLEDupdate
void SSD1305::OLEDBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t *data)
{
	OLED_I2C_ON();
	uint8_t tx, ty;
	uint16_t offset = 0;

	SSD1305_command(SSD1305_SET_COLUMN_ADDR);
	//!EDIT +4 to the next two commands
	// from https://github.com/rm-hull/luma.oled/issues/309
	SSD1305_command(0 + 4);				  // Column start address (0 = reset)
	SSD1305_command(_OLED_WIDTH - 1 + 4); // Column end address (127 = reset)

	SSD1305_command(SSD1305_SET_PAGE_ADDR);
	SSD1305_command(0); // Page start address (0 = reset)

	switch (_OLED_HEIGHT)
	{
	case 64:
		SSD1305_command(7);
		break;
	case 32:
		SSD1305_command(3);
		break;
	case 16:
		SSD1305_command(1);
		break;
	}

	for (ty = 0; ty < h; ty = ty + 8)
	{
		if (y + ty < 0 || y + ty >= _OLED_HEIGHT)
		{
			continue;
		}
		for (tx = 0; tx < w; tx++)
		{

			if (x + tx < 0 || x + tx >= _OLED_WIDTH)
			{
				continue;
			}
			offset = (w * (ty / 8)) + tx;
			SSD1305_data(data[offset++]);
		}
	}
	OLED_I2C_OFF();
}

// Desc: Draws a Pixel to the screen overides the gfx lib if defined
// Passed x and y co-ords and colour of pixel.
void SSD1305::drawPixel(int16_t x, int16_t y, uint8_t color)
{

	if ((x < 0) || (x >= this->bufferWidth) || (y < 0) || (y >= this->bufferHeight))
	{
		return;
	}
	int16_t temp;
	switch (rotation)
	{
	case 1:
		temp = x;
		x = WIDTH - 1 - y;
		y = temp;
		break;
	case 2:
		x = WIDTH - 1 - x;
		y = HEIGHT - 1 - y;
		break;
	case 3:
		temp = x;
		x = y;
		y = HEIGHT - 1 - temp;
		break;
	}
	uint16_t tc = (bufferWidth * (y / 8)) + x;
	switch (color)
	{
	case WHITE:
		this->buffer[tc] |= (1 << (y & 7));
		break;
	case BLACK:
		this->buffer[tc] &= ~(1 << (y & 7));
		break;
	case INVERSE:
		this->buffer[tc] ^= (1 << (y & 7));
		break;
	}
}

void SSD1305::OLED_StartScrollRight(uint8_t start, uint8_t stop)
{
	OLED_I2C_ON();
	SSD1305_command(SSD1305_RIGHT_HORIZONTAL_SCROLL);
	SSD1305_command(0X00);
	SSD1305_command(start); // start page
	SSD1305_command(0X00);
	SSD1305_command(stop); // end page
	SSD1305_command(0X00);
	SSD1305_command(0XFF);
	SSD1305_command(SSD1305_ACTIVATE_SCROLL);
	OLED_I2C_OFF();
}

void SSD1305::OLED_StartScrollLeft(uint8_t start, uint8_t stop)
{
	OLED_I2C_ON();
	SSD1305_command(SSD1305_LEFT_HORIZONTAL_SCROLL);
	SSD1305_command(0X00);
	SSD1305_command(start);
	SSD1305_command(0X00);
	SSD1305_command(stop);
	SSD1305_command(0X00);
	SSD1305_command(0XFF);
	SSD1305_command(SSD1305_ACTIVATE_SCROLL);
	OLED_I2C_OFF();
}

void SSD1305::OLED_StartScrollDiagRight(uint8_t start, uint8_t stop)
{
	OLED_I2C_ON();
	SSD1305_command(SSD1305_SET_VERTICAL_SCROLL_AREA);
	SSD1305_command(0X00);
	SSD1305_command(_OLED_HEIGHT);
	SSD1305_command(SSD1305_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
	SSD1305_command(0X00);
	SSD1305_command(start);
	SSD1305_command(0X00);
	SSD1305_command(stop);
	SSD1305_command(0X01);
	SSD1305_command(SSD1305_ACTIVATE_SCROLL);
	OLED_I2C_OFF();
}

void SSD1305::OLED_StartScrollDiagLeft(uint8_t start, uint8_t stop)
{
	OLED_I2C_ON();
	SSD1305_command(SSD1305_SET_VERTICAL_SCROLL_AREA);
	SSD1305_command(0X00);
	SSD1305_command(_OLED_HEIGHT);
	SSD1305_command(SSD1305_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
	SSD1305_command(0X00);
	SSD1305_command(start);
	SSD1305_command(0X00);
	SSD1305_command(stop);
	SSD1305_command(0X01);
	SSD1305_command(SSD1305_ACTIVATE_SCROLL);
	OLED_I2C_OFF();
}

void SSD1305::OLED_StopScroll(void)
{
	OLED_I2C_ON();
	SSD1305_command(SSD1305_DEACTIVATE_SCROLL);
	OLED_I2C_OFF();
}

// ********************  EOF  *************
