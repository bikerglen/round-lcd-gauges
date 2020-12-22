#ifndef __dialgen_h__
#define __dialgen_h__

void DrawDialBackground (MagickWand *wand);

void DrawDialTicks (MagickWand *wand, 
		const char *color, 
		int numberTicks, float startAngle, float stopAngle,
        float startRadius, float endRadius);

void LabelDialTicks (MagickWand *wand, 
		const char *color, float radius,
		int numberTicks, float startAngle, float stopAngle,
		float startLabel, float stopLabel, const char *formatSpecifier);

void LabelDial (MagickWand *wand, const char *color, const char *label);

void LabelCo2Dial (MagickWand *wand, const char *color);

#endif