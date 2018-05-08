/*
 * gfx.h
 *
 *  Created on: Apr 11, 2018
 *      Author: max
 */

#ifndef GFX_H_
#define GFX_H_

#include <alt_types.h>

typedef struct { // Data stored PER GLYPH
	alt_u16  bitmapOffset;     // Pointer into GFXfont->bitmap
	alt_u8   width, height;    // Bitmap dimensions in pixels
	alt_u8   xAdvance;         // Distance to advance cursor (x axis)
	alt_8    xOffset, yOffset; // Dist from cursor pos to UL corner
} GFXglyph;

typedef struct { // Data stored for FONT AS A WHOLE:
	alt_u8   *bitmap;      // Glyph bitmaps, concatenated
	GFXglyph *glyph;       // Glyph array
	alt_u8    first, last; // ASCII extents
	alt_u8    yAdvance;    // Newline distance (y axis)
} GFXfont;

int gfxInit(void);
int gfxCmd(void);

void writePixel(alt_u16 x, alt_u16 y, alt_u16 color);
void writeLine (alt_u16 x0, alt_u16 y0, alt_u16 x1, alt_u16 y1, alt_u16 color);
void drawRect  (alt_u16 x, alt_u16 y, alt_u16 w, alt_u16 h, alt_u16 color);
void fillRect  (alt_u16 x, alt_u16 y, alt_u16 w, alt_u16 h, alt_u16 color);
void drawCircle(alt_u16 x0, alt_u16 y0, alt_u16 r, alt_u16 color);
void fillCircle(alt_u16 x0, alt_u16 y0, alt_u16 r, alt_u16 color);
int  drawChar  (alt_u16 x, alt_u16 y, alt_u16 color, alt_u8 size, alt_u8 c);
int  drawTxt   (alt_u16 x, alt_u16 y, alt_u16 color, alt_u8* txt);

#endif /* GFX_H_ */
