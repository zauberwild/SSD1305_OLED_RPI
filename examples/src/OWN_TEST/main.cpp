

#include <unistd.h>

#include <bcm2835.h>
#include <time.h>
#include <stdio.h>
#include "SSD1305_OLED.h"
#include "Bitmap_test_data.h" // Test data for bitmaps

#define myOLEDwidth  128
#define myOLEDheight 32
uint8_t fullscreenBuffer[1024];

SSD1305 myOLED(myOLEDwidth ,myOLEDheight) ; // instantiate  an object


int8_t  setup(void);
void EndTests(void);



int main(int argc, char **argv)
{
	if (!setup())
	{
		return -1;
	}
	myOLED.buffer = (uint8_t*) &fullscreenBuffer;
	myOLED.OLEDFillScreen(0x00, 0);
	myOLED.OLEDclearBuffer();


	// myOLED.drawLine(0,0, 128,32, 1);
	// myOLED.drawFastHLine(100, 28, 100, 1);
	// myOLED.drawFastVLine(100, 28, 100, 1);
	// myOLED.fillRect(64,16, 64,16, 1);
	
	myOLED.OLEDupdate();

	printf("displaying done - now waiting\n");
	bcm2835_delay(30 * 1000);
	myOLED.OLEDFillScreen(0x00, 0);
	myOLED.OLEDclearBuffer();


	EndTests();
	return 0;
}



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

	return 1;
}
