#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <ImageMagick-6/wand/MagickWand.h>

#include "ptrgen.h"


void DrawPointerBackground (MagickWand *wand)
{
	PixelWand *background;
	background = NewPixelWand ();
	PixelSetColor (background, "none");
	MagickNewImage (wand, 240, 240, background);
	DestroyPixelWand (background);
}


void DrawPointerKnub (
		MagickWand *wand, const char *color, float radius, float opacity)
{
	DrawingWand *drawingWand;
	PixelWand *strokeColor;
	PixelWand *fillColor;

    drawingWand = NewDrawingWand ();
    strokeColor = NewPixelWand ();
    fillColor = NewPixelWand ();

    PixelSetColor (strokeColor, "none");
    PixelSetColor (fillColor, color);
    DrawSetFillColor (drawingWand, fillColor);
    DrawSetStrokeColor (drawingWand, strokeColor);
	DrawSetStrokeOpacity (drawingWand, 0.0);
	DrawSetStrokeWidth (drawingWand, 0.0);
	DrawSetFillOpacity (drawingWand, opacity);
    DrawCircle (drawingWand, 119.5, 119.5, 120-radius-0.5, 120);

    MagickResetIterator (wand);
    MagickDrawImage (wand, drawingWand);

    DestroyDrawingWand (drawingWand);
    DestroyPixelWand (strokeColor);
    DestroyPixelWand (fillColor);
}


void DrawPointerNeedle (
		MagickWand *wand, const char *color, 
		float strokeWidth, float tipRadius, float tailRadius, float opacity)
{
	DrawingWand *drawingWand;
	PixelWand *strokeColor;

	drawingWand = NewDrawingWand ();
	strokeColor = NewPixelWand ();

	DrawSetStrokeWidth (drawingWand, strokeWidth);
	PixelSetColor (strokeColor, color);
	DrawSetStrokeColor (drawingWand, strokeColor);
	DrawSetStrokeOpacity (drawingWand, opacity);
	DrawLine (drawingWand, 119.5, 120+tailRadius, 119.5, 120-tipRadius);

    MagickResetIterator (wand);
    MagickDrawImage (wand, drawingWand);

    DestroyDrawingWand (drawingWand);
    DestroyPixelWand (strokeColor);
}
