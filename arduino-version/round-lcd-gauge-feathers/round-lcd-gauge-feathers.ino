#include <SPI.h>
#include "images.h"

#define TFT_RST       A0
#define TFT_DC        A1
#define TFT_CS        A2

#define USE_HORIZONTAL 2  // Set the display direction 0,1,2,3 four directions

void InitTftHardware (void);
void GC9A01_Initial (void);
void Write_Cmd_Data (unsigned char);
void Write_Cmd (unsigned char);
void Write_Data (unsigned char DH, unsigned char DL);
void Write_Data_U16 (unsigned int y);
void LCD_SetPos (unsigned int Xstart, unsigned int Ystart, unsigned int Xend, unsigned int Yend);
void delayms (unsigned int tt);

SPISettings settings(32000000, BitOrder::MSBFIRST, SPI_MODE0);

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
  uint32_t *lined;
  float  sina, cosa;

  uint32_t linebuffer[240];

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
    
    //----------------------------------------
    // rotate one line
    //----------------------------------------

    ydif = ycen - i;
    lined = linebuffer;
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

      lines = needle + yp * 240;

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


    //----------------------------------------
    // blend one line
    //----------------------------------------

    const uint32_t *ptrs1;
    const uint32_t *ptrs2;
    uint8_t lcdbuffer[2*240];
    uint8_t *lcdbuffp = lcdbuffer;

    ptrs1 = dial + i*240;
    ptrs2 = linebuffer;
  
    for (int j = 0; j < w; j++) {
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

    //----------------------------------------
    // send one line
    //----------------------------------------

    SPI.transfer (lcdbuffer, 2*240);
  }

  // close lcd writes
  SPI.endTransaction();
  digitalWrite (TFT_CS, 1);
}


void InitTftHardware (void)
{
  pinMode (TFT_RST, OUTPUT);
  pinMode (TFT_DC, OUTPUT);
  pinMode (TFT_CS, OUTPUT);
  SPI.begin ();
  delay (50);
}


void  Write_Cmd_Data (unsigned char CMDP)
{
  digitalWrite (TFT_CS, 0);
  digitalWrite (TFT_DC, 1);
  SPI.beginTransaction(settings);
  SPI.transfer(CMDP);
  SPI.endTransaction();
  digitalWrite (TFT_CS, 1);
}


void Write_Cmd (unsigned char CMD)
{
  digitalWrite (TFT_CS, 0);
  digitalWrite (TFT_DC, 0);
  SPI.beginTransaction(settings);
  SPI.transfer(CMD);
  SPI.endTransaction();
  digitalWrite (TFT_CS, 1);
}


void Write_Data_U16 (unsigned int y)
{
  unsigned char m,n;
  m=y>>8;
  n=y;
  Write_Data(m,n);
}


void Write_Data (unsigned char DH,unsigned char DL)
{
  digitalWrite (TFT_CS, 0);
  digitalWrite (TFT_DC, 1);
  SPI.beginTransaction(settings);
  SPI.transfer(DH);
  SPI.transfer(DL);
  SPI.endTransaction();
  digitalWrite (TFT_CS, 1);
}


void delayms (unsigned int count)
{
  delay (count);
}


void GC9A01_Initial(void)
{ 
  digitalWrite (TFT_CS, 1);
  delayms(5);
  digitalWrite (TFT_RST, 0);
  delayms(10);
  digitalWrite (TFT_RST, 1);
  delayms(120);  
  
  
  Write_Cmd(0xEF);
  
  Write_Cmd(0xEB);
  Write_Cmd_Data(0x14); 
  
  Write_Cmd(0xFE);       
  Write_Cmd(0xEF); 
  
  Write_Cmd(0xEB);  
  Write_Cmd_Data(0x14); 
  
  Write_Cmd(0x84);      
  Write_Cmd_Data(0x40); 
  
  Write_Cmd(0x85);      
  Write_Cmd_Data(0xFF); 
  
  Write_Cmd(0x86);      
  Write_Cmd_Data(0xFF); 
  
  Write_Cmd(0x87);      
  Write_Cmd_Data(0xFF);
  
  Write_Cmd(0x88);      
  Write_Cmd_Data(0x0A);
  
  Write_Cmd(0x89);      
  Write_Cmd_Data(0x21); 
  
  Write_Cmd(0x8A);      
  Write_Cmd_Data(0x00); 
  
  Write_Cmd(0x8B);      
  Write_Cmd_Data(0x80); 
  
  Write_Cmd(0x8C);      
  Write_Cmd_Data(0x01); 
  
  Write_Cmd(0x8D);      
  Write_Cmd_Data(0x01); 
  
  Write_Cmd(0x8E);      
  Write_Cmd_Data(0xFF); 
  
  Write_Cmd(0x8F);      
  Write_Cmd_Data(0xFF); 
  
  Write_Cmd(0xB6);
  Write_Cmd_Data(0x00);       
  Write_Cmd_Data(0x00); 
  
  Write_Cmd(0x36);
  
  if(USE_HORIZONTAL==0)Write_Cmd_Data(0x18);
  else if(USE_HORIZONTAL==1)Write_Cmd_Data(0x28);
  else if(USE_HORIZONTAL==2)Write_Cmd_Data(0x48);
  else Write_Cmd_Data(0x88);
  
  Write_Cmd(0x3A);      
  Write_Cmd_Data(0x05); 
    
  Write_Cmd(0x90);      
  Write_Cmd_Data(0x08);
  Write_Cmd_Data(0x08);
  Write_Cmd_Data(0x08);
  Write_Cmd_Data(0x08); 
  
  Write_Cmd(0xBD);      
  Write_Cmd_Data(0x06);
  
  Write_Cmd(0xBC);      
  Write_Cmd_Data(0x00); 
  
  Write_Cmd(0xFF);      
  Write_Cmd_Data(0x60);
  Write_Cmd_Data(0x01);
  Write_Cmd_Data(0x04);
  
  Write_Cmd(0xC3);      
  Write_Cmd_Data(0x13);
  Write_Cmd(0xC4);      
  Write_Cmd_Data(0x13);
  
  Write_Cmd(0xC9);      
  Write_Cmd_Data(0x22);
  
  Write_Cmd(0xBE);      
  Write_Cmd_Data(0x11); 
  
  Write_Cmd(0xE1);      
  Write_Cmd_Data(0x10);
  Write_Cmd_Data(0x0E);
  
  Write_Cmd(0xDF);      
  Write_Cmd_Data(0x21);
  Write_Cmd_Data(0x0c);
  Write_Cmd_Data(0x02);
  
  Write_Cmd(0xF0);   
  Write_Cmd_Data(0x45);
  Write_Cmd_Data(0x09);
  Write_Cmd_Data(0x08);
  Write_Cmd_Data(0x08);
  Write_Cmd_Data(0x26);
  Write_Cmd_Data(0x2A);
  
  Write_Cmd(0xF1);    
  Write_Cmd_Data(0x43);
  Write_Cmd_Data(0x70);
  Write_Cmd_Data(0x72);
  Write_Cmd_Data(0x36);
  Write_Cmd_Data(0x37);  
  Write_Cmd_Data(0x6F);
  
  Write_Cmd(0xF2);   
  Write_Cmd_Data(0x45);
  Write_Cmd_Data(0x09);
  Write_Cmd_Data(0x08);
  Write_Cmd_Data(0x08);
  Write_Cmd_Data(0x26);
  Write_Cmd_Data(0x2A);
  
  Write_Cmd(0xF3);   
  Write_Cmd_Data(0x43);
  Write_Cmd_Data(0x70);
  Write_Cmd_Data(0x72);
  Write_Cmd_Data(0x36);
  Write_Cmd_Data(0x37); 
  Write_Cmd_Data(0x6F);
  
  Write_Cmd(0xED);  
  Write_Cmd_Data(0x1B); 
  Write_Cmd_Data(0x0B); 
  
  Write_Cmd(0xAE);      
  Write_Cmd_Data(0x77);
  
  Write_Cmd(0xCD);      
  Write_Cmd_Data(0x63);   
  
  Write_Cmd(0x70);      
  Write_Cmd_Data(0x07);
  Write_Cmd_Data(0x07);
  Write_Cmd_Data(0x04);
  Write_Cmd_Data(0x0E); 
  Write_Cmd_Data(0x0F); 
  Write_Cmd_Data(0x09);
  Write_Cmd_Data(0x07);
  Write_Cmd_Data(0x08);
  Write_Cmd_Data(0x03);
  
  Write_Cmd(0xE8);      
  Write_Cmd_Data(0x34);
  
  Write_Cmd(0x62);      
  Write_Cmd_Data(0x18);
  Write_Cmd_Data(0x0D);
  Write_Cmd_Data(0x71);
  Write_Cmd_Data(0xED);
  Write_Cmd_Data(0x70); 
  Write_Cmd_Data(0x70);
  Write_Cmd_Data(0x18);
  Write_Cmd_Data(0x0F);
  Write_Cmd_Data(0x71);
  Write_Cmd_Data(0xEF);
  Write_Cmd_Data(0x70); 
  Write_Cmd_Data(0x70);
  
  Write_Cmd(0x63);      
  Write_Cmd_Data(0x18);
  Write_Cmd_Data(0x11);
  Write_Cmd_Data(0x71);
  Write_Cmd_Data(0xF1);
  Write_Cmd_Data(0x70); 
  Write_Cmd_Data(0x70);
  Write_Cmd_Data(0x18);
  Write_Cmd_Data(0x13);
  Write_Cmd_Data(0x71);
  Write_Cmd_Data(0xF3);
  Write_Cmd_Data(0x70); 
  Write_Cmd_Data(0x70);
  
  Write_Cmd(0x64);      
  Write_Cmd_Data(0x28);
  Write_Cmd_Data(0x29);
  Write_Cmd_Data(0xF1);
  Write_Cmd_Data(0x01);
  Write_Cmd_Data(0xF1);
  Write_Cmd_Data(0x00);
  Write_Cmd_Data(0x07);
  
  Write_Cmd(0x66);      
  Write_Cmd_Data(0x3C);
  Write_Cmd_Data(0x00);
  Write_Cmd_Data(0xCD);
  Write_Cmd_Data(0x67);
  Write_Cmd_Data(0x45);
  Write_Cmd_Data(0x45);
  Write_Cmd_Data(0x10);
  Write_Cmd_Data(0x00);
  Write_Cmd_Data(0x00);
  Write_Cmd_Data(0x00);
  
  Write_Cmd(0x67);      
  Write_Cmd_Data(0x00);
  Write_Cmd_Data(0x3C);
  Write_Cmd_Data(0x00);
  Write_Cmd_Data(0x00);
  Write_Cmd_Data(0x00);
  Write_Cmd_Data(0x01);
  Write_Cmd_Data(0x54);
  Write_Cmd_Data(0x10);
  Write_Cmd_Data(0x32);
  Write_Cmd_Data(0x98);
  
  Write_Cmd(0x74);      
  Write_Cmd_Data(0x10); 
  Write_Cmd_Data(0x85); 
  Write_Cmd_Data(0x80);
  Write_Cmd_Data(0x00); 
  Write_Cmd_Data(0x00); 
  Write_Cmd_Data(0x4E);
  Write_Cmd_Data(0x00);         
  
  Write_Cmd(0x98);      
  Write_Cmd_Data(0x3e);
  Write_Cmd_Data(0x07);
  
  Write_Cmd(0x35);  
  Write_Cmd(0x21);
  
  Write_Cmd(0x11);
  delayms(120);
  Write_Cmd(0x29);
  delayms(20);
}


void LCD_SetPos (unsigned int Xstart,unsigned int Ystart,unsigned int Xend,unsigned int Yend)
{
  Write_Cmd(0x2a);   
  Write_Cmd_Data(Xstart>>8);
  Write_Cmd_Data(Xstart);
  Write_Cmd_Data(Xend>>8);
  Write_Cmd_Data(Xend);

  Write_Cmd(0x2b);   
  Write_Cmd_Data(Ystart>>8);
  Write_Cmd_Data(Ystart);
  Write_Cmd_Data(Yend>>8);
  Write_Cmd_Data(Yend);
  
  Write_Cmd(0x2c);//LCD_WriteCMD(GRAMWR);
}
