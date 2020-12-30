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
	// DRAW PERCENT DIAL
	//========================================

	wand = NewMagickWand ();

	DrawDialBackground (wand);
	DrawDialTicks (wand, "#606060", 1.5, 11, -150, 150, 89, 104);
	DrawDialTicks (wand, "#606060", 1.5, 10, -135, 135, 93, 104);
	DrawDialTicks (wand, "#606060", 1.5, 20, -147, 138, 97, 104);
	DrawDialTicks (wand, "#606060", 1.5, 20, -144, 141, 97, 104);
	DrawDialTicks (wand, "#606060", 1.5, 20, -141, 144, 97, 104);
	DrawDialTicks (wand, "#606060", 1.5, 20, -138, 147, 97, 104);
	LabelDialTicks (wand, "#606060", "Oswald-Light.ttf", 85, 11, -150, 150, 0, 100, "%0.0f");
	LabelDial (wand, "#606060", "%");

	MagickResetIterator (wand);
    MagickWriteImage (wand, "dial-percent.png");

	DestroyMagickWand (wand);


	//========================================
	// DRAW CO2 DIAL
	//========================================

	wand = NewMagickWand ();

	DrawDialBackground (wand);
	DrawDialTicks (wand, "#606060", 1.5, 11, -150, 150, 89, 104);
	DrawDialTicks (wand, "#606060", 1.5, 10, -135, 135, 93, 104);
	DrawDialTicks (wand, "#606060", 1.5, 20, -147, 138, 97, 104);
	DrawDialTicks (wand, "#606060", 1.5, 20, -144, 141, 97, 104);
	DrawDialTicks (wand, "#606060", 1.5, 20, -141, 144, 97, 104);
	DrawDialTicks (wand, "#606060", 1.5, 20, -138, 147, 97, 104);
	LabelDialTicks (wand, "#606060", "Oswald-Light.ttf", 85, 6, -150, 150, 0, 1000, "%0.0f");
	LabelCo2Dial (wand, "#606060");

	MagickResetIterator (wand);
    MagickWriteImage (wand, "dial-co2.png");

	DestroyMagickWand (wand);


	//========================================
	// DRAW BASIC RED NEEDLE
	// A Cool Green: #35BF5F
	// A Cool Red:   #F45700
	//========================================

	wand = NewMagickWand ();

    DrawPointerBackground (wand);
    DrawPointerKnub (wand, "#F45700", 119.5, 119.5, 10, 1);
    DrawPointerNeedle (wand, "#F45700", 2.25, 103, 30, 1);

    MagickResetIterator (wand);
    MagickWriteImage (wand, "pointer-red-basic.png");

    DestroyMagickWand (wand);


	//========================================
	// DRAW BASIC RED NEEDLE SHADOW
	//========================================

    wand = NewMagickWand ();

    DrawPointerBackground (wand);
    DrawPointerKnub (wand, "#808080", 119.5, 119.5, 10, 1);
    DrawPointerNeedle (wand, "#808080", 2.25, 103, 30, 1);

	MagickResetIterator (wand);
    MagickWriteImage (wand, "shadow-red-basic.png");

	DestroyMagickWand (wand);


	//========================================
	// DRAW BLACK ARROW
	// A Cool Green: #35BF5F
	// A Cool Red:   #F45700
	//========================================

	wand = NewMagickWand ();

    DrawPointerBackground (wand);
    DrawPointerKnub (wand, "#202020", 119.5, 119.5, 10, 1);
    DrawPointerArrow (wand, "#202020", 95, 30, 10, 5, 1);

    MagickResetIterator (wand);
    MagickWriteImage (wand, "pointer-black-arrow.png");

    DestroyMagickWand (wand);


	//========================================
	// DRAW BLACK ARROW SHADOW
	//========================================

    wand = NewMagickWand ();

    DrawPointerBackground (wand);
    DrawPointerKnub (wand, "#808080", 119.5, 119.5, 10, 1);
    DrawPointerArrow (wand, "#808080", 95, 30, 10, 5, 1);

	MagickResetIterator (wand);
    MagickWriteImage (wand, "shadow-black-arrow.png");

	DestroyMagickWand (wand);


	// we're done here
	MagickWandTerminus ();

	return 0;
}



