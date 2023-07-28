#define TFT_RST       33
#define TFT_DC        22
#define TFT_CS        19
#define TFT_GDAT      21
#define TFT_GCLK      25
 
// 32000000 works well for M4 and STM32F4 feathers
// 80000000 should work for ESP32 feather
#define TFT_SPI_SPEED 80000000


// 0 == bottom at header connector
// 1 == left at header connector
// 2 == top at header connector
// 3 == bottom at header connector
// 4 == right at header connector
#define USE_HORIZONTAL 1  // Set the display direction 0,1,2,3 four directions

void InitTftHardware (void);
void GC9A01_Initial (void);
void Write_Cmd_Data (unsigned char);
void Write_Cmd (unsigned char);
void Write_Data (unsigned char DH, unsigned char DL);
void Write_Data_U16 (unsigned int y);
void LCD_SetPos (unsigned int Xstart, unsigned int Ystart, unsigned int Xend, unsigned int Yend);
void delayms (unsigned int tt);
