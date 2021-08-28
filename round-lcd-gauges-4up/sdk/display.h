#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#define TFT_RST 0
#define TFT_DC  1
#define TFT_CS  2

void GPIO_write (int select, uint8_t pin, uint8_t value);

// these functions more or less copied directly from the GC9A01 example code
void GC9A01_Initial (int select);
void Write_Cmd_Data (int select, unsigned char);
void Write_Cmd (int select, unsigned char);
void Write_Data (int select, unsigned char DH, unsigned char DL);
void LCD_SetPos (int select, unsigned int Xstart,unsigned int Ystart,unsigned int Xend,unsigned int Yend);

#endif
