Basic gauge code

Displays the a gauge with the dial, needle, and shadow PNG files specified on the command line.

Uses pigpio and leptonica libraries

sudo apt install libleptonica-dev
sudo apt install libpigpio-dev

Run make to compile then sudo basic <dial png> <pointer png> [<shadow png>] to run.

hardware:

TFT-SCL             // PIN 19 -- SPI CLK
TFT-SDA             // PIN 23 -- SPI SDO
TFT-RST        24   // PIN 18 -- GPIO 24
TFT-DC         25   // PIN 22 -- GPIO 25
TFT-CS          8   // PIN 24 -- GPIO  8

