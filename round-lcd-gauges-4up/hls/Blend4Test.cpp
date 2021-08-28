#include <stdint.h>
#include <memory.h>
#include "ap_int.h"
#include "hls_stream.h"
#include "Blend4.h"

void BlendReference (uint8_t *lcdbuff, const uint32_t *background, const uint32_t *foreground);

extern uint32_t dialCo2[57600];
extern uint32_t needleRed[57600];

uint8_t lcdbuff[2*240*240];

int main (int argc, char *argv[])
{
	uint32_t background[4*57600];
	uint32_t foreground[4*57600];

	hls::stream<uint16_t,57600> pixels0;
	hls::stream<uint16_t,57600> pixels1;
	hls::stream<uint16_t,57600> pixels2;
	hls::stream<uint16_t,57600> pixels3;

	int compares;
	int errors;
	uint8_t *lcdbuffptr;

	int totalErrors;

	memcpy (background+0*57600, dialCo2, 4*57600);
	memcpy (background+1*57600, dialCo2, 4*57600);
	memcpy (background+2*57600, dialCo2, 4*57600);
	memcpy (background+3*57600, dialCo2, 4*57600);

	memcpy (foreground+0*57600, needleRed, 4*57600);
	memcpy (foreground+1*57600, needleRed, 4*57600);
	memcpy (foreground+2*57600, needleRed, 4*57600);
	memcpy (foreground+3*57600, needleRed, 4*57600);

	BlendReference (lcdbuff, dialCo2, needleRed);

	Blend4 (pixels0, pixels1, pixels2, pixels3, background, foreground);

	// zero total error count
	totalErrors = 0;


	//----------------------------------------
	// compare first output
	//----------------------------------------

	compares = 0;
	errors = 0;
	lcdbuffptr = lcdbuff;
	for (int row = 0; row < 240; row++) {
		for (int col = 0; col < 240; col++) {
			compares++;
			uint16_t refPixel = ((*lcdbuffptr++) << 8) | (*lcdbuffptr++);
			uint16_t dutPixel = pixels0.read ();
			if (dutPixel != refPixel) {
				errors++;
				printf ("row %3d, col %3d: ref: %04x, dut: %04x\n", row, col, refPixel, dutPixel);
			}
		}
	}
	printf ("first output: compares: %d, errors: %d\n", compares, errors);
	totalErrors += errors;


	//----------------------------------------
	// compare second output
	//----------------------------------------

	compares = 0;
	errors = 0;
	lcdbuffptr = lcdbuff;
	for (int row = 0; row < 240; row++) {
		for (int col = 0; col < 240; col++) {
			compares++;
			uint16_t refPixel = ((*lcdbuffptr++) << 8) | (*lcdbuffptr++);
			uint16_t dutPixel = pixels1.read ();
			if (dutPixel != refPixel) {
				errors++;
				printf ("row %3d, col %3d: ref: %04x, dut: %04x\n", row, col, refPixel, dutPixel);
			}
		}
	}
	printf ("second output: compares: %d, errors: %d\n", compares, errors);
	totalErrors += errors;


	//----------------------------------------
	// compare third output
	//----------------------------------------

	compares = 0;
	errors = 0;
	lcdbuffptr = lcdbuff;
	for (int row = 0; row < 240; row++) {
		for (int col = 0; col < 240; col++) {
			compares++;
			uint16_t refPixel = ((*lcdbuffptr++) << 8) | (*lcdbuffptr++);
			uint16_t dutPixel = pixels2.read ();
			if (dutPixel != refPixel) {
				errors++;
				printf ("row %3d, col %3d: ref: %04x, dut: %04x\n", row, col, refPixel, dutPixel);
			}
		}
	}
	printf ("third output: compares: %d, errors: %d\n", compares, errors);
	totalErrors += errors;


	//----------------------------------------
	// compare fourth output
	//----------------------------------------

	compares = 0;
	errors = 0;
	lcdbuffptr = lcdbuff;
	for (int row = 0; row < 240; row++) {
		for (int col = 0; col < 240; col++) {
			compares++;
			uint16_t refPixel = ((*lcdbuffptr++) << 8) | (*lcdbuffptr++);
			uint16_t dutPixel = pixels3.read ();
			if (dutPixel != refPixel) {
				errors++;
				printf ("row %3d, col %3d: ref: %04x, dut: %04x\n", row, col, refPixel, dutPixel);
			}
		}
	}
	printf ("fourth output: compares: %d, errors: %d\n", compares, errors);
	totalErrors += errors;

	return totalErrors ? -1 : 0;
}


void BlendReference (uint8_t *lcdbuff, const uint32_t *background, const uint32_t *foreground)
{
	// blend dial and rotated needle back into rotated needle
	const uint32_t *ptrs1 = background;
	const uint32_t *ptrs2 = foreground;
	uint8_t *lcdbuffp = lcdbuff;

	for (int y = 0; y < 240; y++) {
		for (int x = 0; x < 240; x++) {
			uint32_t rgba_background = *ptrs1++;
			uint32_t rgba_foreground = *ptrs2++;

			float alpha = (float)(rgba_foreground & 0xff) / 255.0;

			uint8_t rbg = rgba_background >> 24;
			uint8_t gbg = rgba_background >> 16;
			uint8_t bbg = rgba_background >>  8;
			uint8_t rfg = rgba_foreground >> 24;
			uint8_t gfg = rgba_foreground >> 16;
			uint8_t bfg = rgba_foreground >>  8;

			uint8_t r = ((1.0 - alpha) * rbg + alpha * rfg);
			uint8_t g = ((1.0 - alpha) * gbg + alpha * gfg);
			uint8_t b = ((1.0 - alpha) * bbg + alpha * bfg);

			*lcdbuffp++ = (r & 0xf8) | (g >> 5);
			*lcdbuffp++ = ((g & 0x1c) << 3) | (b >> 3);
		}
	}
}
