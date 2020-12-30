#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <ImageMagick-6/wand/MagickWand.h>

#include "dialgen.h"


void DrawDialBackground (MagickWand *wand)
{
	PixelWand *background;
	DrawingWand *drawingWand;
	PixelWand *strokeColor;
	PixelWand *fillColor;

	background = NewPixelWand ();
	PixelSetColor (background, "#ffffff");
	MagickNewImage (wand, 240, 240, background);

	drawingWand = NewDrawingWand ();
	strokeColor = NewPixelWand ();
	fillColor = NewPixelWand ();

	DrawSetStrokeAntialias (drawingWand, MagickTrue);
	DrawSetStrokeWidth (drawingWand, 1);
	PixelSetColor (strokeColor, "#d0d0d0");
	PixelSetColor (fillColor, "#f0f0f0");
	DrawSetStrokeColor (drawingWand, strokeColor);
	DrawSetFillColor (drawingWand, fillColor);
	DrawCircle (drawingWand, 120, 120, 1, 120);

	PixelSetColor (strokeColor, "#e8e8e8");
	DrawSetStrokeColor (drawingWand, strokeColor);
	DrawSetFillColor (drawingWand, strokeColor);
	DrawCircle (drawingWand, 120, 120, 2, 120);

	PixelSetColor (strokeColor, "#d8d8d8");
	DrawSetStrokeColor (drawingWand, strokeColor);
	DrawSetFillColor (drawingWand, strokeColor);
	DrawCircle (drawingWand, 120, 120, 5, 120);

	PixelSetColor (strokeColor, "#d0d0d0");
	DrawSetStrokeColor (drawingWand, strokeColor);
	DrawSetFillColor (drawingWand, strokeColor);
	DrawCircle (drawingWand, 120, 120, 6, 120);

	PixelSetColor (strokeColor, "#c8c8c8");
	DrawSetStrokeColor (drawingWand, strokeColor);
	DrawSetFillColor (drawingWand, strokeColor);
	DrawCircle (drawingWand, 120, 120, 7, 120);

	PixelSetColor (strokeColor, "#c0c0c0");
	DrawSetStrokeColor (drawingWand, strokeColor);
	DrawSetFillColor (drawingWand, strokeColor);
	DrawCircle (drawingWand, 120, 120, 8, 120);

	PixelSetColor (strokeColor, "#e0e0e0");
	DrawSetStrokeColor (drawingWand, strokeColor);
	DrawSetFillColor (drawingWand, strokeColor);
	DrawCircle (drawingWand, 120, 120, 9, 120);

	PixelSetColor (strokeColor, "#e0e0e0");
	DrawSetStrokeColor (drawingWand, strokeColor);
	DrawSetFillColor (drawingWand, strokeColor);
	DrawCircle (drawingWand, 120, 120, 10, 120);

	PixelSetColor (strokeColor, "#e0e0e0");
	DrawSetStrokeColor (drawingWand, strokeColor);
	DrawSetFillColor (drawingWand, strokeColor);
	DrawCircle (drawingWand, 120, 120, 11, 120);

	PixelSetColor (strokeColor, "#c8c8c8");
	DrawSetStrokeColor (drawingWand, strokeColor);
	DrawSetFillColor (drawingWand, fillColor);
	DrawCircle (drawingWand, 120, 120, 12, 120);

	MagickResetIterator (wand);
	MagickDrawImage (wand, drawingWand);

	DestroyDrawingWand (drawingWand);
	DestroyPixelWand (background);
	DestroyPixelWand (strokeColor);
	DestroyPixelWand (fillColor);
}


void DrawDialTicks (MagickWand *wand, 
		const char *color, float strokeWidth,
		int numberTicks, float startAngle, float stopAngle,
        float startRadius, float endRadius)
{
	DrawingWand *drawingWand;
	PixelWand *strokeColor;
	int i;
	float angle, angleCorrected, angleRadians;
	float x1, y1, x2, y2;

	drawingWand = NewDrawingWand ();
	strokeColor = NewPixelWand ();

	for (i = 0; i < numberTicks; i++) {
		angle = startAngle + (float)i * (stopAngle - startAngle) / (numberTicks - 1);
		angleCorrected = angle - 90;
		angleRadians = angleCorrected / 180.0 * M_PI;
		printf ("angle = %f\n", angle);
		printf ("corrected = %f\n", angleCorrected);
		printf ("radians = %f\n", angleRadians);

		x1 = 120 + cos (angleRadians) * startRadius;
		y1 = 120 + sin (angleRadians) * startRadius;
		x2 = 120 + cos (angleRadians) * endRadius;
		y2 = 120 + sin (angleRadians) * endRadius;

		DrawSetStrokeAntialias (drawingWand, MagickTrue);
		DrawSetStrokeWidth (drawingWand, strokeWidth);
		PixelSetColor (strokeColor, color);
		DrawSetStrokeColor (drawingWand, strokeColor);
		DrawLine (drawingWand, x1, y1, x2, y2);
	}

	MagickResetIterator (wand);
	MagickDrawImage (wand, drawingWand);

	DestroyDrawingWand (drawingWand);
	DestroyPixelWand (strokeColor);
}


void LabelDialTicks (MagickWand *wand, 
		const char *color, const char *font, float radius,
		int numberTicks, float startAngle, float stopAngle,
		float startLabel, float stopLabel, const char *formatSpecifier)
{
	DrawingWand *drawingWand;
	PixelWand *fillColor;
	int i;
	float angle, angleCorrected, angleRadians;
	float x1, y1, x2, y2;
	float labelValue;
	char labelString[256];
	double *metrics, ascent, descent, width, height;

	drawingWand = NewDrawingWand ();
	fillColor = NewPixelWand ();

	PixelSetColor (fillColor, color);
	DrawSetFont (drawingWand, font);
	DrawSetTextAlignment (drawingWand, CenterAlign);
	DrawSetFontSize (drawingWand, 22);
	DrawSetFillColor (drawingWand, fillColor);

	for (i = 0; i < numberTicks; i++) {
		angle = startAngle + (float)i * (stopAngle - startAngle) / (numberTicks - 1);
		angleCorrected = angle - 90;
		angleRadians = angleCorrected / 180.0 * M_PI;

		labelValue = startLabel + (float)i * (stopLabel - startLabel) / (numberTicks - 1); 
		sprintf (labelString, formatSpecifier, labelValue);
		printf ("label = '%s'\n", labelString);

		metrics = MagickQueryFontMetrics (wand, drawingWand, labelString);
		ascent = metrics[2];
		descent = metrics[3];
		width = metrics[4];
		height = metrics[5];

		// using just ascent does not cause font to be centered at 0 and Pi 
		// radians. ascent += descent seems to work ok for Oswald-Light
		ascent += descent; 

		double roffset = sqrt ( (cos(angleRadians)*width/2) *
		                        (cos(angleRadians)*width/2) +
		                        (sin(angleRadians)*ascent/2) *
		                        (sin(angleRadians)*ascent/2) );

		x1 = 120 + cos (angleRadians) * (radius - roffset);
		y1 = 120 + sin (angleRadians) * (radius - roffset) + ascent/2.0;
		DrawAnnotation (drawingWand, x1, y1, (const unsigned char *)labelString);

		free (metrics);
	}

	MagickResetIterator (wand);
	MagickDrawImage (wand, drawingWand);

	DestroyDrawingWand (drawingWand);
	DestroyPixelWand (fillColor);
}


void LabelDial (MagickWand *wand, const char *color, const char *label)
{
	DrawingWand *drawingWand;
	PixelWand *fillColor;
	int i;
	float angle, angleCorrected, angleRadians;
	float x1, y1, x2, y2;
	float labelValue;
	char labelString[256];
	double *metrics, ascent, descent, width, height;

	drawingWand = NewDrawingWand ();
	fillColor = NewPixelWand ();

	PixelSetColor (fillColor, color);
	DrawSetFont (drawingWand, "Oswald-Light.ttf");
	DrawSetTextAlignment (drawingWand, CenterAlign);
    DrawSetFontSize (drawingWand, 22);
	DrawSetFillColor (drawingWand, fillColor);

    DrawAnnotation (drawingWand, 120, 164, (unsigned char*)label);

	MagickResetIterator (wand);
	MagickDrawImage (wand, drawingWand);

	DestroyDrawingWand (drawingWand);
	DestroyPixelWand (fillColor);
}


void LabelCo2Dial (MagickWand *wand, const char *color)
{
	DrawingWand *drawingWand;
	PixelWand *fillColor;
	int i;
	float angle, angleCorrected, angleRadians;
	float x1, y1, x2, y2;
	float labelValue;
	char labelString[256];
	double *metrics, ascent, descent, width, height;

	drawingWand = NewDrawingWand ();
	fillColor = NewPixelWand ();

	PixelSetColor (fillColor, color);
	DrawSetFont (drawingWand, "Oswald-Light.ttf");
	DrawSetTextAlignment (drawingWand, CenterAlign);
    DrawSetFontSize (drawingWand, 22);
	DrawSetFillColor (drawingWand, fillColor);

    DrawSetFontSize (drawingWand, 18);
    DrawAnnotation (drawingWand, 120, 160, (unsigned char*)"CO   (ppm)");
    DrawSetFontSize (drawingWand, 16);
    DrawAnnotation (drawingWand, 109, 167, (unsigned char*)"2");

	MagickResetIterator (wand);
	MagickDrawImage (wand, drawingWand);

	DestroyDrawingWand (drawingWand);
	DestroyPixelWand (fillColor);
}

