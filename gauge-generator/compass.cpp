#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string>
#include <ImageMagick-6/wand/MagickWand.h>

#include "dialgen.h"
#include "ptrgen.h"

const char *labels[12] = {
	"N",  "30",  "60", 
	"E", "120", "150",
	"S", "210", "240",
	"W", "300", "330"
};

int main (int argc, char *argv[])
{
	MagickWand *wand;
    PixelWand *background;


	//========================================
	// DRAW DIAL
	//========================================

	wand = NewMagickWand ();

    background = NewPixelWand ();
    PixelSetColor (background, "#000000");
    MagickNewImage (wand, 240, 240, background);

    DrawDialTicks (wand, "#c0c0c0", 1.5, 12, -180, 150, 101, 116);
    DrawDialTicks (wand, "#c0c0c0", 1.5, 12, -165, 165, 105, 116);
    DrawDialTicks (wand, "#c0c0c0", 1.5, 24, -177, 168, 109, 116);
    DrawDialTicks (wand, "#c0c0c0", 1.5, 24, -174, 171, 109, 116);
    DrawDialTicks (wand, "#c0c0c0", 1.5, 24, -171, 174, 109, 116);
    DrawDialTicks (wand, "#c0c0c0", 1.5, 24, -168, 177, 109, 116);
	LabelDialTicks2 (wand, "#c0c0c0", "Oswald-Light.ttf", 22, 77, 12, 0, 330, labels);

	// TODO

	MagickResetIterator (wand);
    MagickWriteImage (wand, "compass-dial.png");

	DestroyMagickWand (wand);


	//========================================
	// DRAW NEEDLE
	//========================================

	wand = NewMagickWand ();

	// TODO

	MagickResetIterator (wand);
    MagickWriteImage (wand, "compass-needle.png");

	DestroyMagickWand (wand);


	// we're done here
	MagickWandTerminus ();

	return 0;
}



