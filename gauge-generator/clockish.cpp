#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <ImageMagick-6/wand/MagickWand.h>

#include "dialgen.h"
#include "ptrgen.h"


int main (int argc, char *argv[])
{
	MagickWand *wand;

	//========================================
	// DRAW CLOCKISH DIAL
	//========================================

	wand = NewMagickWand ();

	DrawDialBackground (wand);
	DrawDialTicks (wand, "#363636", 6.0, 11, -150, 150, 79, 104);
	DrawDialTicks (wand, "#363636", 2.5, 10, -144, 126, 97, 104);
	DrawDialTicks (wand, "#363636", 2.5, 10, -138, 132, 97, 104);
	DrawDialTicks (wand, "#363636", 2.5, 10, -132, 138, 97, 104);
	DrawDialTicks (wand, "#363636", 2.5, 10, -126, 144, 97, 104);
	LabelDialTicks (wand, "#363636", "Oswald-Bold.ttf", 72, 11, -150, 150, 0, 10, "%0.0f");

	MagickResetIterator (wand);
    MagickWriteImage (wand, "dial-clockish.png");

	DestroyMagickWand (wand);


	//========================================
	// DRAW CLOCKISH POINTER
	// A Cool Green: #35BF5F
	// A Cool Red:   #F45700
	//========================================

	wand = NewMagickWand ();

    DrawPointerBackground (wand);
    // DrawPointerKnub (wand, "#ED3655", 119.5, 57, 12.5, 1);
    DrawPointerCircle (wand, "#ED3655", 119.5, 57, 12.5, 3.0, 1);
    DrawPointerNeedle (wand, "#ED3655", 3.0, 50, 25, 1);
    DrawPointerNeedle (wand, "#ED3655", 3.0, 96, -75, 1);

    MagickResetIterator (wand);
    MagickWriteImage (wand, "pointer-clockish.png");

    DestroyMagickWand (wand);


	// we're done here
	MagickWandTerminus ();

	return 0;
}



