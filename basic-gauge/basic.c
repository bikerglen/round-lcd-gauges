//=============================================================================================
// Basic Gauge Code
//


//---------------------------------------------------------------------------------------------
// includes
//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <leptonica/allheaders.h>
#include <pigpio.h>

#include "display.h"


//---------------------------------------------------------------------------------------------
// defines
//


//---------------------------------------------------------------------------------------------
// prototypes
//

void SetPointerAngle (PIX *dial, PIX *pointer, PIX *shadow, float angle);


//---------------------------------------------------------------------------------------------
// globals
//

// handle to the rpi spi interface
int spi;


int main (int argc, char *argv[])
{
	PIX *dial = NULL;
	PIX *pointer = NULL;
	PIX *shadow = NULL;

	if (argc < 3) {
		printf ("usage: basic <dial.png> <pointer.png> [<shadow.png>]\n");
		printf ("try:\n");
		printf ("  basic ../gauge-generator/dial-co2.png ../gauge-generator/pointer-red-basic.png\n");
		printf ("or:\n");
		printf ("  basic ../gauge-generator/dial-co2.png ../gauge-generator/pointer-red-basic.png ../gauge-generator/shadow-red-basic.png\n");

		return -1;
	}

	// initialize pigpio
	if (gpioInitialise () < 0) {
		printf ("failed to initialize gpio\n");
		return -1;
	}	

	// intialize non-spi gpio pins
	gpioSetMode (TFT_RST, PI_OUTPUT);
	gpioWrite   (TFT_RST, 0);
	gpioSetMode (TFT_DC, PI_OUTPUT);
	gpioWrite   (TFT_DC, 0);
	gpioSetMode (TFT_CS, PI_OUTPUT);
	gpioWrite   (TFT_CS, 1);

	// initialize spi
	spi = spiOpen (0, 32000000, 0xE0);

	// wait 50 ms
	usleep (50000);

	// initialize dipslay controller
	GC9A01_Initial ();

	// read dial, pointer, and shadow
	printf ("Reading dial...\n");
	dial = pixRead (argv[1]);
	printf ("Reading pointer...\n");
	pointer = pixRead (argv[2]);
	if (argc >= 4) {
		printf ("Reading shadow...\n");
		shadow = pixRead (argv[3]);
	} else {
		shadow = NULL;
	}

	// check dial size
	if ((dial->w != 240) || (dial->h != 240)) {
		printf ("dial must be exactly 240x240 pixels\n");
		return -1;
	}

	// check pointer size
	if ((pointer->w != 240) || (pointer->h != 240)) {
		printf ("pointer must be exactly 240x240 pixels\n");
		return -1;
	}

	// check shadow size if it exists
	if (shadow && ((shadow->w != 240) || (shadow->h != 240))) {
		printf ("shadow must be exactly 240x240 pixels\n");
		return -1;
	}

	// imagemagick was using different grays/alphas for the knub vs the needle with anything 
	// other than an opacity of 1.0. So the shadow as read in above has an opacity of 1.0
	// wherever there's pixels.  the code below changes this to 50% so it's least partially 
	// transparent when rendered.
	if (shadow != NULL) {
		uint32_t *sptr = shadow->data;
		for (int y = 0; y < 240; y++) {
			for (int x = 0; x < 240; x++) {
				*sptr = (*sptr & 0xFFFFFF00) + ((*sptr & 0xFF)>>1);
				sptr++;
			}
		}
	}
	
	// move needle from minimum to maximum and back continuously
	int direction = 1;
	while (1) {
		for (int angle = -150; angle <= +150; angle++) {
			// apply direction to angle
			float realAngle = direction * angle;

			// render pointer and copy to display
			SetPointerAngle (dial, pointer, shadow, realAngle);
		}
		// change direction
		direction *= -1;
	}

	if (dial != NULL) { pixDestroy (&dial); }
	if (pointer != NULL) { pixDestroy (&pointer); }
	if (shadow != NULL) { pixDestroy (&shadow); }

	return 0;
}


void SetPointerAngle (PIX *dial, PIX *pointer, PIX *shadow, float angle)
{
	float radians;
	uint8_t imageData[115200];

	PIX *dest1 = NULL; 
	PIX *dest2 = NULL; 
	PIX *dest3 = NULL; 
	PIX *dest4 = NULL;

	// convert angle to radians
	radians = angle/180.8*M_PI;

	// rotate pointer
	dest1 = pixRotateAMColor (pointer, radians, 0xFFFFFF00);

	// see if shadow needs to be rendered too
	if (shadow != NULL) {
		// rotate shadow
		dest3 = pixRotateAMColor (shadow, radians, 0xFFFFFF00);
		uint32_t *sptr = dest3->data;
		// fix up white corners that may have rotated into view
		for (int y = 0; y < 240; y++) {
			for (int x = 0; x < 240; x++) {
				if (*sptr == 0xFFFFFFFF) {
					*sptr = 0x00000000;
				}
				sptr++;
			}
		}
		// blend rotated shadowed into dial background
		dest4 = pixBlendWithGrayMask (dial, dest3, NULL, 3, 3);
		// blend rotated needle into the above
		dest2 = pixBlendWithGrayMask (dest4, dest1, NULL, 0, 0);
	} else {
		// blend rotated needle into dial background
		dest2 = pixBlendWithGrayMask (dial, dest1, NULL, 0, 0);
	}

	// convert 32 bit RGBA color into 16 bit R5G6B5 color
	uint32_t *sptr = dest2->data;
	uint8_t *dptr = imageData;
	for (int y = 0; y < 240; y++) {
		for (int x = 0; x < 240; x++) {
			// RRRRRRRR GGGGGGGG BBBBBBBB AAAAAAAA
			// 76543210 76543210 76543210 76543210
			//                   RRRRRGGG GGGBBBBB
			uint32_t rgba = *sptr++;
			uint8_t  r = (rgba >> 27) & 0x1f;
			uint8_t  g = (rgba >> 18) & 0x3f;
			uint8_t  b = (rgba >> 11) & 0x1f;
			uint16_t r5g6b5 = (r << 11) | (g << 5) | b;
			*dptr++ = (r5g6b5 >> 8) & 0xff;
			*dptr++ = (r5g6b5 >> 0) & 0xff;
		}
	}

	// copy display data to lcd
	int n = 0;
	LCD_SetPos(0,0,239,239);
	gpioWrite (TFT_CS, 0);
	gpioWrite (TFT_DC, 1);
	for(int j=0;j<240;j++)
	{
		spiWrite (spi, (char *)&imageData[n], 480);
		n += 480;
	}
	gpioWrite (TFT_CS, 1);

	if (dest1 != NULL) { pixDestroy (&dest1); }
	if (dest2 != NULL) { pixDestroy (&dest2); }
	if (dest3 != NULL) { pixDestroy (&dest3); }
	if (dest4 != NULL) { pixDestroy (&dest4); }
}
