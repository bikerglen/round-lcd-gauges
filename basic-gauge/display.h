#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#define TFT_RST        24   // PIN 18 -- GPIO 24
#define TFT_DC         25   // PIN 22 -- GPIO 25
#define TFT_CS          8   // PIN 24 -- GPIO  8

#define USE_HORIZONTAL  2   // set the display direction 0,1,2,3

// these functions more or less copied directly from the GC9A01 example code
void GC9A01_Initial (void);
void Write_Cmd_Data (unsigned char);
void Write_Cmd (unsigned char);
void Write_Data (unsigned char DH, unsigned char DL);
void delayms (unsigned int tt);
void Write_Data_U16 (unsigned int y);
void LCD_SetPos (unsigned int Xstart,unsigned int Ystart,unsigned int Xend,unsigned int Yend);
void ClearScreen (unsigned int bColor);

// my functions
void SPI_transfer (uint8_t a);

#endif
