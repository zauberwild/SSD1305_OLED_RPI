/*
* Project Name: SSD1305_OLED_RPI
* Author: Gavin Lyons.
* URL: https://github.com/gavinlyonsrepo/SSD1305_OLED_RPI
*/
  
#ifndef _SSD1305_OLED_H_
#define _SSD1305_OLED_H_

// ************** Library INCLUDES ********
#include <bcm2835.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "SSD1305_OLED_graphics.h"

// ********* DEFINES ********
// ***********  SSD1305 Command Set  ***********

// Fundamental Commands
#define SSD1305_SET_CONTRAST_CONTROL                    0x81
#define SSD1305_DISPLAY_ALL_ON_RESUME                   0xA4
#define SSD1305_DISPLAY_ALL_ON                          0xA5
#define SSD1305_NORMAL_DISPLAY                          0xA6
#define SSD1305_INVERT_DISPLAY                          0xA7
#define SSD1305_DISPLAY_OFF                             0xAE
#define SSD1305_DISPLAY_ON                              0xAF
#define SSD1305_NOP                                     0xE3

// Scrolling Commands
#define SSD1305_RIGHT_HORIZONTAL_SCROLL                 0x26
#define SSD1305_LEFT_HORIZONTAL_SCROLL                  0x27
#define SSD1305_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL    0x29
#define SSD1305_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL     0x2A
#define SSD1305_DEACTIVATE_SCROLL                       0x2E
#define SSD1305_ACTIVATE_SCROLL                         0x2F
#define SSD1305_SET_VERTICAL_SCROLL_AREA                0xA3

// Addressing Setting Commands
#define SSD1305_SET_LOWER_COLUMN                        0x00
#define SSD1305_SET_HIGHER_COLUMN                       0x10
#define SSD1305_MEMORY_ADDR_MODE                        0x20
#define SSD1305_SET_COLUMN_ADDR                         0x21
#define SSD1305_SET_PAGE_ADDR                           0x22

// Hardware Configuration Commands
#define SSD1305_SET_START_LINE                          0x40
#define SSD1305_SET_SEGMENT_REMAP                       0xA0
#define SSD1305_SET_MULTIPLEX_RATIO                     0xA8
#define SSD1305_COM_SCAN_DIR_INC                        0xC0
#define SSD1305_COM_SCAN_DIR_DEC                        0xC8
#define SSD1305_SET_DISPLAY_OFFSET                      0xD3
#define SSD1305_SET_COM_PINS                            0xDA
#define SSD1305_CHARGE_PUMP                             0x8D

// Timing & Driving Scheme Setting Commands
#define SSD1305_SET_DISPLAY_CLOCK_DIV_RATIO             0xD5
#define SSD1305_SET_PRECHARGE_PERIOD                    0xD9
#define SSD1305_SET_VCOM_DESELECT                       0xDB

// I2C related
#define SSD1305_COMMAND                     0x00
#define SSD1305_DATA                        0xC0
#define SSD1305_DATA_CONTINUE               0x40
#define SSD1305_ADDR                        0x3C // 0x3D
#define SSD1305_command(Reg)  I2C_Write_Byte(Reg, SSD1305_COMMAND)
#define SSD1305_data(Data)    I2C_Write_Byte(Data, SSD1305_DATA_CONTINUE)

#define BLACK                 0
#define WHITE                 1
#define INVERSE               2

// Delays
#define SSD1305_INITDELAY 100 // mS


// *********** CLASS SECTION ***********8
class SSD1305 : public SSD1305_graphics  {
  public:
	SSD1305(int16_t oledwidth, int16_t oledheight);
	~SSD1305(){};

	uint8_t* buffer = nullptr;

	virtual void drawPixel(int16_t x, int16_t y, uint8_t color) override;
	void OLEDupdate(void);
	void OLEDclearBuffer(void);
	void OLEDBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t* data);
	void OLEDFillScreen(uint8_t pixel, uint8_t mircodelay);
	void OLEDFillPage(uint8_t page_num, uint8_t pixels,uint8_t delay);
	void OLEDBitmap(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t* data, bool invert);
	
	void OLEDbegin(void);
	void OLEDinit(void);
	void OLEDReset(void);
	void OLEDPowerDown(void);
	
	void OLEDEnable(uint8_t on);
	void OLEDContrast(uint8_t OLEDcontrast);
	void OLEDInvert(bool on);
	
	void OLED_StartScrollRight(uint8_t start, uint8_t stop); 
	void OLED_StartScrollLeft(uint8_t start, uint8_t stop) ;
	void OLED_StartScrollDiagRight(uint8_t start, uint8_t stop) ;
	void OLED_StartScrollDiagLeft(uint8_t start, uint8_t stop) ;
	void OLED_StopScroll(void) ;
	

  private:

	void I2C_Write_Byte(uint8_t value, uint8_t Cmd);
	void OLED_I2C_OFF(void);
	void OLED_I2C_ON(void);
	
	int16_t _OLED_WIDTH;
	int16_t _OLED_HEIGHT;
	int8_t _OLED_PAGE_NUM;
	uint8_t bufferWidth ;
	uint8_t bufferHeight ;

}; // end of class

#endif // end of guard header

