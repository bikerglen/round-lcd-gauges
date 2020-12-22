Install the latest versions of imagemagick and libmagickwand-dev to build and use this code.

sudo apt install imagemagick
sudo apt install libmagickwand-dev

dialgen.cpp and dialgen.h contain the utility functions to create gauge backgrounds.

ptrgen.cpp and ptrgen.h contain the utility functions to build simple gauge needles.

examples.cpp uses the dialgen and ptrgen code to build several example gauges. 

run make then execute examples to build the gauges.

The Oswald-Light.ttf font is required to run the dialgen code. It may be downloaded from fontsquirrel.com at https://www.fontsquirrel.com/fonts/oswald.

Output files are (for now):

Backgrounds:

dial-co2.png
dial-percent.png

Needles:

pointer-red-basic.png
shadow-red-basic.png

test
