Basic display test code

Displays two images in an infinite loop. My truck in Wyoming and me in the desert somewhere.

Uses pigpio and leptonica libraries

sudo apt install libleptonica-dev
sudo apt install libpigpio-dev

Run make to compile then sudo disptest to run.

hardware:

TFT-SCL             // PIN 19 -- SPI CLK
TFT-SDA             // PIN 23 -- SPI SDO
TFT-RST        24   // PIN 18 -- GPIO 24
TFT-DC         25   // PIN 22 -- GPIO 25
TFT-CS          8   // PIN 24 -- GPIO  8

