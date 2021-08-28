#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "xparameters.h"
#include "xil_io.h"

#include "blend4_hw.h"
#include "display.h"

#define USE_HORIZONTAL  2   // set the display direction 0,1,2,3

static void SPI_transfer (int select, uint8_t a);
static void delayms (unsigned int count);


//===============================================================
//lcd initial
void GC9A01_Initial (int select)
{
	GPIO_write (select, TFT_CS, 1);
	delayms(5);
	GPIO_write (select, TFT_RST, 0);
	delayms(10);
	GPIO_write (select, TFT_RST, 1);
	delayms(120);

	Write_Cmd(select, 0xEF);

	Write_Cmd(select, 0xEB);
	Write_Cmd_Data(select, 0x14);

	Write_Cmd(select, 0xFE);
	Write_Cmd(select, 0xEF);

	Write_Cmd(select, 0xEB);
	Write_Cmd_Data(select, 0x14);

	Write_Cmd(select, 0x84);
	Write_Cmd_Data(select, 0x40);

	Write_Cmd(select, 0x85);
	Write_Cmd_Data(select, 0xFF);

	Write_Cmd(select, 0x86);
	Write_Cmd_Data(select, 0xFF);

	Write_Cmd(select, 0x87);
	Write_Cmd_Data(select, 0xFF);

	Write_Cmd(select, 0x88);
	Write_Cmd_Data(select, 0x0A);

	Write_Cmd(select, 0x89);
	Write_Cmd_Data(select, 0x21);

	Write_Cmd(select, 0x8A);
	Write_Cmd_Data(select, 0x00);

	Write_Cmd(select, 0x8B);
	Write_Cmd_Data(select, 0x80);

	Write_Cmd(select, 0x8C);
	Write_Cmd_Data(select, 0x01);

	Write_Cmd(select, 0x8D);
	Write_Cmd_Data(select, 0x01);

	Write_Cmd(select, 0x8E);
	Write_Cmd_Data(select, 0xFF);

	Write_Cmd(select, 0x8F);
	Write_Cmd_Data(select, 0xFF);


	Write_Cmd(select, 0xB6);
	Write_Cmd_Data(select, 0x00);
	Write_Cmd_Data(select, 0x00);

	Write_Cmd(select, 0x36);

	if(USE_HORIZONTAL==0)Write_Cmd_Data(select, 0x18);
	else if(USE_HORIZONTAL==1)Write_Cmd_Data(select, 0x28);
	else if(USE_HORIZONTAL==2)Write_Cmd_Data(select, 0x48);
	else Write_Cmd_Data(select, 0x88);



	Write_Cmd(select, 0x3A);
	Write_Cmd_Data(select, 0x05);


	Write_Cmd(select, 0x90);
	Write_Cmd_Data(select, 0x08);
	Write_Cmd_Data(select, 0x08);
	Write_Cmd_Data(select, 0x08);
	Write_Cmd_Data(select, 0x08);

	Write_Cmd(select, 0xBD);
	Write_Cmd_Data(select, 0x06);

	Write_Cmd(select, 0xBC);
	Write_Cmd_Data(select, 0x00);

	Write_Cmd(select, 0xFF);
	Write_Cmd_Data(select, 0x60);
	Write_Cmd_Data(select, 0x01);
	Write_Cmd_Data(select, 0x04);

	Write_Cmd(select, 0xC3);
	Write_Cmd_Data(select, 0x13);
	Write_Cmd(select, 0xC4);
	Write_Cmd_Data(select, 0x13);

	Write_Cmd(select, 0xC9);
	Write_Cmd_Data(select, 0x22);

	Write_Cmd(select, 0xBE);
	Write_Cmd_Data(select, 0x11);

	Write_Cmd(select, 0xE1);
	Write_Cmd_Data(select, 0x10);
	Write_Cmd_Data(select, 0x0E);

	Write_Cmd(select, 0xDF);
	Write_Cmd_Data(select, 0x21);
	Write_Cmd_Data(select, 0x0c);
	Write_Cmd_Data(select, 0x02);

	Write_Cmd(select, 0xF0);
	Write_Cmd_Data(select, 0x45);
	Write_Cmd_Data(select, 0x09);
	Write_Cmd_Data(select, 0x08);
	Write_Cmd_Data(select, 0x08);
	Write_Cmd_Data(select, 0x26);
	Write_Cmd_Data(select, 0x2A);

	Write_Cmd(select, 0xF1);
	Write_Cmd_Data(select, 0x43);
	Write_Cmd_Data(select, 0x70);
	Write_Cmd_Data(select, 0x72);
	Write_Cmd_Data(select, 0x36);
	Write_Cmd_Data(select, 0x37);
	Write_Cmd_Data(select, 0x6F);


	Write_Cmd(select, 0xF2);
	Write_Cmd_Data(select, 0x45);
	Write_Cmd_Data(select, 0x09);
	Write_Cmd_Data(select, 0x08);
	Write_Cmd_Data(select, 0x08);
	Write_Cmd_Data(select, 0x26);
	Write_Cmd_Data(select, 0x2A);

	Write_Cmd(select, 0xF3);
	Write_Cmd_Data(select, 0x43);
	Write_Cmd_Data(select, 0x70);
	Write_Cmd_Data(select, 0x72);
	Write_Cmd_Data(select, 0x36);
	Write_Cmd_Data(select, 0x37);
	Write_Cmd_Data(select, 0x6F);

	Write_Cmd(select, 0xED);
	Write_Cmd_Data(select, 0x1B);
	Write_Cmd_Data(select, 0x0B);

	Write_Cmd(select, 0xAE);
	Write_Cmd_Data(select, 0x77);

	Write_Cmd(select, 0xCD);
	Write_Cmd_Data(select, 0x63);


	Write_Cmd(select, 0x70);
	Write_Cmd_Data(select, 0x07);
	Write_Cmd_Data(select, 0x07);
	Write_Cmd_Data(select, 0x04);
	Write_Cmd_Data(select, 0x0E);
	Write_Cmd_Data(select, 0x0F);
	Write_Cmd_Data(select, 0x09);
	Write_Cmd_Data(select, 0x07);
	Write_Cmd_Data(select, 0x08);
	Write_Cmd_Data(select, 0x03);

	Write_Cmd(select, 0xE8);
	Write_Cmd_Data(select, 0x34);

	Write_Cmd(select, 0x62);
	Write_Cmd_Data(select, 0x18);
	Write_Cmd_Data(select, 0x0D);
	Write_Cmd_Data(select, 0x71);
	Write_Cmd_Data(select, 0xED);
	Write_Cmd_Data(select, 0x70);
	Write_Cmd_Data(select, 0x70);
	Write_Cmd_Data(select, 0x18);
	Write_Cmd_Data(select, 0x0F);
	Write_Cmd_Data(select, 0x71);
	Write_Cmd_Data(select, 0xEF);
	Write_Cmd_Data(select, 0x70);
	Write_Cmd_Data(select, 0x70);

	Write_Cmd(select, 0x63);
	Write_Cmd_Data(select, 0x18);
	Write_Cmd_Data(select, 0x11);
	Write_Cmd_Data(select, 0x71);
	Write_Cmd_Data(select, 0xF1);
	Write_Cmd_Data(select, 0x70);
	Write_Cmd_Data(select, 0x70);
	Write_Cmd_Data(select, 0x18);
	Write_Cmd_Data(select, 0x13);
	Write_Cmd_Data(select, 0x71);
	Write_Cmd_Data(select, 0xF3);
	Write_Cmd_Data(select, 0x70);
	Write_Cmd_Data(select, 0x70);

	Write_Cmd(select, 0x64);
	Write_Cmd_Data(select, 0x28);
	Write_Cmd_Data(select, 0x29);
	Write_Cmd_Data(select, 0xF1);
	Write_Cmd_Data(select, 0x01);
	Write_Cmd_Data(select, 0xF1);
	Write_Cmd_Data(select, 0x00);
	Write_Cmd_Data(select, 0x07);

	Write_Cmd(select, 0x66);
	Write_Cmd_Data(select, 0x3C);
	Write_Cmd_Data(select, 0x00);
	Write_Cmd_Data(select, 0xCD);
	Write_Cmd_Data(select, 0x67);
	Write_Cmd_Data(select, 0x45);
	Write_Cmd_Data(select, 0x45);
	Write_Cmd_Data(select, 0x10);
	Write_Cmd_Data(select, 0x00);
	Write_Cmd_Data(select, 0x00);
	Write_Cmd_Data(select, 0x00);

	Write_Cmd(select, 0x67);
	Write_Cmd_Data(select, 0x00);
	Write_Cmd_Data(select, 0x3C);
	Write_Cmd_Data(select, 0x00);
	Write_Cmd_Data(select, 0x00);
	Write_Cmd_Data(select, 0x00);
	Write_Cmd_Data(select, 0x01);
	Write_Cmd_Data(select, 0x54);
	Write_Cmd_Data(select, 0x10);
	Write_Cmd_Data(select, 0x32);
	Write_Cmd_Data(select, 0x98);

	Write_Cmd(select, 0x74);
	Write_Cmd_Data(select, 0x10);
	Write_Cmd_Data(select, 0x85);
	Write_Cmd_Data(select, 0x80);
	Write_Cmd_Data(select, 0x00);
	Write_Cmd_Data(select, 0x00);
	Write_Cmd_Data(select, 0x4E);
	Write_Cmd_Data(select, 0x00);

	Write_Cmd(select, 0x98);
	Write_Cmd_Data(select, 0x3e);
	Write_Cmd_Data(select, 0x07);

	Write_Cmd(select, 0x35);
	Write_Cmd(select, 0x21);

	Write_Cmd(select, 0x11);
	delayms(120);
	Write_Cmd(select, 0x29);
	delayms(20);
}


//===============================================================
//write parameter
void  Write_Cmd_Data (int select, unsigned char CMDP)
{
	GPIO_write (select, TFT_CS, 0);
	GPIO_write (select, TFT_DC, 1);
	SPI_transfer (select, CMDP);
	GPIO_write (select, TFT_CS, 1);
}


//=============================================================
//write command

void Write_Cmd(int select, unsigned char CMD)
{
	GPIO_write (select, TFT_CS, 0);
	GPIO_write (select, TFT_DC, 0);
	SPI_transfer (select, CMD);
	GPIO_write (select, TFT_CS, 1);
}


//===================================================================
//write data byte

void Write_Data(int select, unsigned char DH,unsigned char DL)
{
	GPIO_write (select, TFT_CS, 0);
	GPIO_write (select, TFT_DC, 1);
	SPI_transfer(select, DH);
	SPI_transfer(select, DL);
	GPIO_write (select, TFT_CS, 1);
}


//===============================================================
//lcd set position
void LCD_SetPos(int select, unsigned int Xstart,unsigned int Ystart,unsigned int Xend,unsigned int Yend)
{
	Write_Cmd(select, 0x2a);
	Write_Cmd_Data(select, Xstart>>8);
	Write_Cmd_Data(select, Xstart);
	Write_Cmd_Data(select, Xend>>8);
	Write_Cmd_Data(select, Xend);

	Write_Cmd(select, 0x2b);
	Write_Cmd_Data(select, Ystart>>8);
	Write_Cmd_Data(select, Ystart);
	Write_Cmd_Data(select, Yend>>8);
	Write_Cmd_Data(select, Yend);

	Write_Cmd(select, 0x2c);//LCD_WriteCMD(GRAMWR);
}


//===============================================================
//GPIO_write

void GPIO_write (int select, uint8_t pin, uint8_t value)
{
	uint32_t address;

	switch (select) {
		case 0: address = XPAR_LCD_SPI_0_BASEADDR + 0x18; break;
		case 1: address = XPAR_LCD_SPI_1_BASEADDR + 0x18; break;
		case 2: address = XPAR_LCD_SPI_2_BASEADDR + 0x18; break;
		case 3: address = XPAR_LCD_SPI_3_BASEADDR + 0x18; break;
		default: address = XPAR_LCD_SPI_0_BASEADDR + 0x18; break;
	}

	switch (pin) {
		case TFT_RST:
			if (value) {
				(*((volatile uint32_t *)(address))) |= 0x4;
			} else {
				(*((volatile uint32_t *)(address))) &= ~0x4;
			}
			break;
		case TFT_DC:
			if (value) {
				(*((volatile uint32_t *)(address))) |= 0x2;
			} else {
				(*((volatile uint32_t *)(address))) &= ~0x2;
			}
			break;
		case TFT_CS:
			if (value) {
				(*((volatile uint32_t *)(address))) |= 0x1;
			} else {
				(*((volatile uint32_t *)(address))) &= ~0x1;
			}
			break;
	}
}


//===============================================================
//SPI_transfer
static void SPI_transfer (int select, uint8_t a)
{
	uint32_t status;
	uint32_t data;

	switch (select) {
		case 0:
			data   = XPAR_LCD_SPI_0_BASEADDR + 0x1C;
			status = XPAR_LCD_SPI_0_BASEADDR + 0x20;
			break;
		case 1:
			data   = XPAR_LCD_SPI_1_BASEADDR + 0x1C;
			status = XPAR_LCD_SPI_1_BASEADDR + 0x20;
			break;
		case 2:
			data   = XPAR_LCD_SPI_2_BASEADDR + 0x1C;
			status = XPAR_LCD_SPI_2_BASEADDR + 0x20;
			break;
		case 3:
			data   = XPAR_LCD_SPI_3_BASEADDR + 0x1C;
			status = XPAR_LCD_SPI_3_BASEADDR + 0x20;
			break;
		default:
			data   = XPAR_LCD_SPI_0_BASEADDR + 0x1C;
			status = XPAR_LCD_SPI_0_BASEADDR + 0x20;
			break;
	}


	// wait while 8-bit data input fifo is full
	while ((*((volatile uint32_t *)(status))) & 0x01) {
	}

	// write data to FIFO
	(*((volatile uint32_t *)(data))) = a;

	// wait for serializer in use flag to clear
	while ((*((volatile uint32_t *)(status))) & 0x10) {
	}
}


//===============================================================
//delayms
static void delayms (unsigned int count)
{
	usleep (count * 1000);
}


