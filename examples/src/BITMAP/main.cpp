
// Example file name : main.cpp
// Description: Test file showing how to display bitmaps.
// URL: https://github.com/gavinlyonsrepo/SSD1305_OLED_RPI
// *****************************


#include <bcm2835.h>
#include <time.h>
#include <stdio.h>
#include "SSD1305_OLED.h"
#include "Bitmap_test_data.h"

#define myOLEDwidth  128
#define myOLEDheight 64
uint8_t fullscreenBuffer[1024];

SSD1305 myOLED(myOLEDwidth ,myOLEDheight) ; // instantiate  an object

// =============== Function prototype ================
int8_t  setup(void);
void myLoop(void);
void Test3(void);
void Test2(void);
void Test1(void);
void EndTests(void);

// ======================= Main ===================
int main(int argc, char **argv)
{
	if (!setup())
	{
		return -1;
	}
	myLoop();
	EndTests();
	return 0;
}
// ======================= End of Main ===================


// ======================= Function space ===================
void EndTests()
{
	myOLED.OLEDPowerDown(); //Switch off display
	bcm2835_close(); // Close the library
	printf("OLED End\r\n");
}

int8_t setup()
{
	if(!bcm2835_init())
	{
		printf("Error 1201 Cannot init bcm2835 library\n");
		return -1;
	}
	bcm2835_delay(50);
	printf("OLED Begin\r\n");
	myOLED.OLEDbegin(); // initialize the OLED
	myOLED.OLEDFillScreen(0x01, 0); //splash screen
	bcm2835_delay(1500);
	return 1;
}

void myLoop()
{
	Test1();  
	Test2();  
	Test3();  
}


void Test1(void)
{
	myOLED.buffer = (uint8_t*) &fullscreenBuffer;
	myOLED.OLEDBitmap(0, 0 , 64, 64, bigImage, false);
	myOLED.OLEDupdate();
	
	bcm2835_delay(5000);
	myOLED.OLEDFillScreen(0x00, 0);
	myOLED.OLEDclearBuffer();
}

void Test2(void)
{
	myOLED.OLEDBitmap(30, 10, 20, 20, smallImage, true);
	myOLED.OLEDBitmap(70, 10, 16, 8,  SignalIcon, false);
	myOLED.OLEDupdate();
	
	bcm2835_delay(5000);
	myOLED.OLEDFillScreen(0x00, 0);
	myOLED.OLEDclearBuffer();

}

void Test3(void)
{
	myOLED.OLEDBitmap(0, 0, myOLEDwidth, myOLEDheight, fullscreenBitmap, true);
	myOLED.OLEDupdate();
	
	bcm2835_delay(5000);
	myOLED.OLEDFillScreen(0x00, 0);
}

