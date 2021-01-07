#ifndef __PTRGEN__
#define __PTRGEN__

void DrawPointerBackground (MagickWand *wand);

void DrawPointerKnub (
		MagickWand *wand, const char *color, float centerx, float centery,
		float radius, float opacity);

void DrawPointerCircle (
        MagickWand *wand, const char *color, float centerx, float centery,
        float radius, float strokeWidth, float opacity);

void DrawPointerNeedle (
		MagickWand *wand, const char *color, 
		float strokeWidth, float tipRadius, float tailRadius, float opacity);

void DrawPointerArrow (
        MagickWand *wand, const char *color,
        float tipRadius, float tailRadius, 
		float width, float tailBackoff, float opacity);

#endif
