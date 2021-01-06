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
  int32_t rbval, gaval;
  uint32_t word00, word01, word10, word11;
  const uint32_t *lines;
  float  sina, cosa;
  const uint32_t mask = 0x00ff00ff;
  
  const uint32_t *ptrs1;
  uint32_t rgba_background;
  uint32_t rb_background;
  uint32_t ga_background;
  uint8_t lcdbuffer[2*240];
  uint8_t *lcdbuffp;
  uint8_t alpha;

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
        // if off the edge, set to transparent
        rbval = 0;
        gaval = 0;
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

        rbval =
               ((16 - xf) * (16 - yf) * ((word00 >> 8) & mask) +
                xf * (16 - yf) * ((word10 >> 8) & mask) +
                (16 - xf) * yf * ((word01 >> 8) & mask) +
                xf * yf * ((word11 >> 8) & mask) + 128 + (128<<16)) >> 8;

        gaval =
               ((16 - xf) * (16 - yf) * (word00 & mask) +
                xf * (16 - yf) * (word10 & mask) +
                (16 - xf) * yf * (word01 & mask) +
                xf * yf * (word11 & mask) + 128 + (128<<16)) >> 8;
      }
      

      //----------------------------------------
      // blend
      //----------------------------------------

      rgba_background = *ptrs1++;

      rb_background = (rgba_background >> 8) & mask;
      ga_background = rgba_background & mask;

      alpha = gaval;

      if (alpha != 0) {
        rbval &= mask;
        gaval &= mask;
        rb_background = ((255 - alpha) * rb_background + alpha * rbval) >> 8;
        ga_background = ((255 - alpha) * ga_background + alpha * gaval) >> 8;
      }

      *lcdbuffp++ = ((rb_background >> 16) & 0xf8) | (ga_background >> 21);
      *lcdbuffp++ = ((ga_background >> 13) & 0xE0) | ((rb_background >> 3) & 0x1f);
      
    } // j

    SPI.transfer (lcdbuffer, 2*240);
    
  } // i

  // close lcd writes
  SPI.endTransaction();
  digitalWrite (TFT_CS, 1);
}
