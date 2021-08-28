#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include <memory.h>
#include "platform.h"
#include "xil_printf.h"
#include "xgpio.h"
#include "xparameters.h"
#include "xil_io.h"

#include "blend4_hw.h"
#include "images.h"
#include "display.h"

void DisplayGauge (const uint32_t *dial, const uint32_t *needle, float angle);
void Blend4 (uint8_t *lcdbuff, const uint32_t *background, const uint32_t *foreground);
void Rotate (uint32_t *dest, const uint32_t *src, float angle);

// pixel buffer for 4 backgrounds
uint32_t backgrounds[4*57600];

// pixel buffer for 4 rotated pointers
uint32_t rotatedNeedles[4*57600];

int main()
{
	uint32_t temp;
	XGpio gpio;
	u32 btn, led;

	init_platform ();

	xil_printf ("\n\r\n\rHello, world!\n\r");
	xil_printf ("LCD 0 Test Reg 0: %08x\n\r", LCD0_SPI_TEST_0);
	xil_printf ("LCD 1 Test Reg 1: %08x\n\r", LCD1_SPI_TEST_0);
	xil_printf ("LCD 2 Test Reg 2: %08x\n\r", LCD2_SPI_TEST_0);
	xil_printf ("LCD 3 Test Reg 3: %08x\n\r", LCD3_SPI_TEST_0);

	XGpio_Initialize (&gpio, 0);
	XGpio_SetDataDirection (&gpio, 2, 0x00000000); // set LED GPIO channel tristates to All Output
	XGpio_SetDataDirection (&gpio, 1, 0xFFFFFFFF); // set BTN GPIO channel tristates to All Input

	// set resets low and chip selects high on all four LCD GPIO modules
	// bits are { rst_n, dc, cs_n }
	LCD0_SPI_GPIO = 0x01;
	LCD1_SPI_GPIO = 0x01;
	LCD2_SPI_GPIO = 0x01;
	LCD3_SPI_GPIO = 0x01;

	// wait 50ms
	usleep (50000);

	// initialize display controllers
	GC9A01_Initial (0);
	GC9A01_Initial (1);
	GC9A01_Initial (2);
	GC9A01_Initial (3);

	xil_printf ("Finished system initialization.\n\r");

	memcpy (backgrounds + 0*57600, dialCo2, 4*57600);
	memcpy (backgrounds + 1*57600, dialClockish, 4*57600);
	memcpy (backgrounds + 2*57600, dialPercent, 4*57600);
	memcpy (backgrounds + 3*57600, dialCo2, 4*57600);

	xil_printf ("Finished building array of backgrounds.\n\r");

	while (1) {
		for (int angle = 0; angle < 360; angle++) {
			float angle0 = (angle +   0) / 180.0 * M_PI;
			float angle1 = (angle +  90) / 180.0 * M_PI;
			float angle2 = (angle + 180) / 180.0 * M_PI;
			float angle3 = (angle + 270) / 180.0 * M_PI;

			Rotate (&rotatedNeedles[0*57600], needleRed, angle0);
			Rotate (&rotatedNeedles[1*57600], pointerClockish, angle1);
			Rotate (&rotatedNeedles[2*57600], pointerBlack, angle2);
			Rotate (&rotatedNeedles[3*57600], needleRed, angle3);

			BLEND_AP_BKGND_0 = (u32)(backgrounds);
			BLEND_AP_BKGND_1 = 0;

			BLEND_AP_FRGND_0 = (u32)(rotatedNeedles);
			BLEND_AP_FRGND_1 = 0;

			LCD_SetPos (0, 0,0,239,239);
			LCD_SetPos (1, 0,0,239,239);
			LCD_SetPos (2, 0,0,239,239);
			LCD_SetPos (3, 0,0,239,239);

			GPIO_write (0, TFT_CS, 0);
			GPIO_write (0, TFT_DC, 1);
			GPIO_write (1, TFT_CS, 0);
			GPIO_write (1, TFT_DC, 1);
			GPIO_write (2, TFT_CS, 0);
			GPIO_write (2, TFT_DC, 1);
			GPIO_write (3, TFT_CS, 0);
			GPIO_write (3, TFT_DC, 1);

			// xil_printf ("starting blend\n\r");
			temp = BLEND_AP_CTRL & 0x80;
			BLEND_AP_CTRL = temp | 0x01;
			// xil_printf ("blend started\n\r");

			// wait for idle to come back
			while (!(BLEND_AP_CTRL & 4)) { }

			// wait for LCD3 d16 fifo empty and LCD3 spi idle to avoid de-asserting the
			// chip selects before the last few lines of data are sent to the display
			while ((LCD3_SPI_SRLZ_STAT & 0x0C) != 0x04) { }

			GPIO_write (0, TFT_CS, 1);
			GPIO_write (1, TFT_CS, 1);
			GPIO_write (2, TFT_CS, 1);
			GPIO_write (3, TFT_CS, 1);
		}
	}

	while (1)
	{
		btn = XGpio_DiscreteRead(&gpio, 1);

		if (btn != 0) // turn all LEDs on when any button is pressed
			led = 0xFFFFFFFF;
		else
			led = 0x00000000;

		XGpio_DiscreteWrite(&gpio, 2, led);

		xil_printf("\rbutton state: %08x", btn);
	}

	cleanup_platform();
    return 0;
}





// area map rotate code from leptonica by Dan Bloomberg
// see https://github.com/DanBloomberg/leptonica/blob/master/src/rotateam.c
// for license and disclaimer

void Rotate (uint32_t *dest, const uint32_t *src, float angle)
{
	const int32_t w = 240;
	const int32_t h = 240;

	int32_t  i, j, xcen, ycen, wm2, hm2;
	int32_t  xdif, ydif, xpm, ypm, xp, yp, xf, yf;
	int32_t  rval, gval, bval, aval;
	uint32_t word00, word01, word10, word11;
	const uint32_t *lines;
	uint32_t *lined;
	float  sina, cosa;

    xcen = w / 2;
    wm2 = w - 2;
    ycen = h / 2;
    hm2 = h - 2;
    sina = 16. * sin(angle);
    cosa = 16. * cos(angle);

    for (i = 0; i < h; i++) {
        ydif = ycen - i;
        lined = dest + i * 240;
        for (j = 0; j < w; j++) {
            xdif = xcen - j;
            xpm = (int32_t)(-xdif * cosa - ydif * sina);
            ypm = (int32_t)(-ydif * cosa + xdif * sina);
            xp = xcen + (xpm >> 4);
            yp = ycen + (ypm >> 4);
            xf = xpm & 0x0f;
            yf = ypm & 0x0f;

                /* if off the edge, write input colorval */
            if (xp < 0 || yp < 0 || xp > wm2 || yp > hm2) {
                *(lined + j) = 0x00000000;
                continue;
            }

            lines = src + yp * 240;

                /* do area weighting.  Without this, we would
                 * simply do:
                 *   *(lined + j) = *(lines + xp);
                 * which is faster but gives lousy results!
                 */
            word00 = *(lines + xp);
            word10 = *(lines + xp + 1);
            word01 = *(lines + 240 + xp);
            word11 = *(lines + 240 + xp + 1);
            rval = ((16 - xf) * (16 - yf) * ((word00 >> 24) & 0xff) +
                    xf * (16 - yf) * ((word10 >> 24) & 0xff) +
                    (16 - xf) * yf * ((word01 >> 24) & 0xff) +
                    xf * yf * ((word11 >> 24) & 0xff) + 128) / 256;
            gval = ((16 - xf) * (16 - yf) * ((word00 >> 16) & 0xff) +
                    xf * (16 - yf) * ((word10 >> 16) & 0xff) +
                    (16 - xf) * yf * ((word01 >> 16) & 0xff) +
                    xf * yf * ((word11 >> 16) & 0xff) + 128) / 256;
            bval = ((16 - xf) * (16 - yf) * ((word00 >> 8) & 0xff) +
                    xf * (16 - yf) * ((word10 >> 8) & 0xff) +
                    (16 - xf) * yf * ((word01 >> 8) & 0xff) +
                    xf * yf * ((word11 >> 8) & 0xff) + 128) / 256;
            aval = ((16 - xf) * (16 - yf) * ((word00 >> 0) & 0xff) +
                    xf * (16 - yf) * ((word10 >> 0) & 0xff) +
                    (16 - xf) * yf * ((word01 >> 0) & 0xff) +
                    xf * yf * ((word11 >> 0) & 0xff) + 128) / 256;
            *(lined+j) = (rval << 24) | (gval << 16) | (bval << 8) | aval;
        }
    }
}
