#ifndef __PTRGEN__
#define __PTRGEN__

void DrawPointerBackground (MagickWand *wand);

void DrawPointerKnub (
		MagickWand *wand, const char *color, float radius, float opacity);

void DrawPointerNeedle (
		MagickWand *wand, const char *color, 
		float strokeWidth, float tipRadius, float tailRadius, float opacity);

#endif
