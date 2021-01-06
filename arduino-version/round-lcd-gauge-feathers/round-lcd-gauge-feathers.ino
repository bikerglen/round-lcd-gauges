#include <SPI.h>
#include "display.h"
#include "images.h"

extern SPISettings settings;

void DisplayGauge (const uint32_t *dial, const uint32_t *needle, float angle);

void setup (void)
{
  InitTftHardware ();  
  GC9A01_Initial ();
}

int direction = 1;
int angle = -150;

void loop (void)
{
  float realAngle = direction * angle / 180.0 * M_PI;
  DisplayGauge (dialPercent, needleRed, realAngle);

  angle++;
  if (angle >= 150) {
    direction *= -1;
    angle = -150;
  }
}


// line by line version to conserve RAM; see original commented out code below this function
// area map rotate code from leptonica by Dan Bloomberg
// see https://github.com/DanBloomberg/leptonica/blob/master/src/rotateam.c
// for license and disclaimer

void DisplayGauge (const uint32_t *dial, const uint32_t *needle, float angle)
{
  const int32_t w = 240;
  const int32_t h = 240;

  int32_t  i, j, xcen, ycen, wm2, hm2;
  int32_t  xdif, ydif, xpm, ypm, xp, yp, xf, yf;
  int32_t  rval, gval, bval, aval;
  uint32_t word00, word01, word10, word11;
  const uint32_t *lines;
  float  sina, cosa;

  const uint32_t *ptrs1;
  uint8_t lcdbuffer[2*240];
  uint8_t *lcdbuffp;

  // start writes to LCD
  LCD_SetPos (0, 0, 239, 239);
  digitalWrite (TFT_CS, 0);
  digitalWrite (TFT_DC, 1);
  SPI.beginTransaction(settings);

  // prelim rotate calculations
  xcen = w / 2;
  wm2 = w - 2;
  ycen = h / 2;
  hm2 = h - 2;
  sina = 16. * sin(angle);
  cosa = 16. * cos(angle);

  // for each line
  for (i = 0; i < h; i++) {

    ptrs1 = dial + i*240; // pick background line
    lcdbuffp = lcdbuffer;
    
    ydif = ycen - i;
    for (j = 0; j < w; j++) {
      xdif = xcen - j;
      xpm = (int32_t)(-xdif * cosa - ydif * sina);
      ypm = (int32_t)(-ydif * cosa + xdif * sina);
      xp = xcen + (xpm >> 4);
      yp = ycen + (ypm >> 4);
      xf = xpm & 0x0f;
      yf = ypm & 0x0f;

      //----------------------------------------
      // rotate
      //----------------------------------------

      if (xp < 0 || yp < 0 || xp > wm2 || yp > hm2) {
        // if off the edge, write input colorval
        rval = 0;
        gval = 0;
        bval = 0;
        aval = 0;
      } else {
        // do area weighting.  Without this, we would
        // simply do:
        //   *(lined + j) = *(lines + xp);
        // which is faster but gives lousy results!

        lines = needle + yp * 240;
  
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
      }
      
      //----------------------------------------
      // blend
      //----------------------------------------

      uint32_t rgba_background = *ptrs1++;

      uint8_t rbg = rgba_background >> 24;
      uint8_t gbg = rgba_background >> 16;
      uint8_t bbg = rgba_background >>  8;

      uint8_t r = ((255 - aval) * rbg + aval * rval) >> 8;
      uint8_t g = ((255 - aval) * gbg + aval * gval) >> 8;
      uint8_t b = ((255 - aval) * bbg + aval * bval) >> 8;

      *lcdbuffp++ = (r & 0xf8) | (g >> 5);
      *lcdbuffp++ = ((g & 0x1c) << 3) | (b >> 3);
      
    } // j

    SPI.transfer (lcdbuffer, 2*240);
    
  } // i

  // close lcd writes
  SPI.endTransaction();
  digitalWrite (TFT_CS, 1);
}
