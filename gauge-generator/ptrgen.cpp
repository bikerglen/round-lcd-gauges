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
		MagickWand *wand, const char *color, float centerx, float centery,
		float radius, float opacity)
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
    DrawCircle (drawingWand, centerx, centery, centerx-radius, centery);

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
	DrawLine (drawingWand, 119.5, 119.5+tailRadius, 119.5, 119.5-tipRadius);

    MagickResetIterator (wand);
    MagickDrawImage (wand, drawingWand);

    DestroyDrawingWand (drawingWand);
    DestroyPixelWand (strokeColor);
}


void DrawPointerArrow (
		MagickWand *wand, const char *color, 
		float tipRadius, float tailRadius, 
		float width, float tailBackoff, float opacity)
{
	DrawingWand *drawingWand;
	PixelWand *fillColor;

	drawingWand = NewDrawingWand ();
	fillColor = NewPixelWand ();

	PixelSetColor (fillColor, color);
	DrawSetStrokeWidth (drawingWand, 0);
	DrawSetFillColor (drawingWand, fillColor);
	DrawSetFillOpacity (drawingWand, opacity);
	
	PointInfo coords[4] = {
		{ 119.5, 120-tipRadius },
		{ 119.5+width/2, 119.5 + tailRadius - tailBackoff },
		{ 119.5, 120+tailRadius },
		{ 119.5-width/2, 119.5 + tailRadius - tailBackoff }
	};

	DrawPolygon (drawingWand, 4, coords);

    MagickResetIterator (wand);
    MagickDrawImage (wand, drawingWand);

    DestroyDrawingWand (drawingWand);
    DestroyPixelWand (fillColor);
}
