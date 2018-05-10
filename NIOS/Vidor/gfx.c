/*
 * gfx.c
 *
 *  Created on: Apr 11, 2018
 *      Author: max
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <alt_types.h>
#include <system.h>

#include "gfx.h"

#define FB_BASE (alt_u16*)(SDRAM_ARBITER_BASE + SDRAM_ARBITER_FB_OFFSET*sizeof(short))
#define FB_WIDTH  640
#define FB_HEIGHT 480

#define _swap_int16_t(a,b) {int16_t t=a; a=b; b=t;}

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


alt_u32 writePixel(alt_u16 x, alt_u16 y, alt_u16 color);
alt_u32 writeLine (alt_u16 x0, alt_u16 y0, alt_u16 x1, alt_u16 y1, alt_u16 color);
alt_u32 drawRect  (alt_u16 x, alt_u16 y, alt_u16 w, alt_u16 h, alt_u16 color);
alt_u32 fillRect  (alt_u16 x, alt_u16 y, alt_u16 w, alt_u16 h, alt_u16 color);
alt_u32 drawCircle(alt_u16 x0, alt_u16 y0, alt_u16 r, alt_u16 color);
alt_u32 fillCircle(alt_u16 x0, alt_u16 y0, alt_u16 r, alt_u16 color);
alt_u32  drawChar  (alt_u16 x, alt_u16 y, alt_u16 color, alt_u8 size, alt_u8 c);
alt_u32  drawTxt   (alt_u16 x, alt_u16 y, alt_u16 color, alt_u8* txt);
typedef void (*pfv3)(alt_u32, alt_u32, alt_u32);
typedef void (*pfv4)(alt_u32, alt_u32, alt_u32, alt_u32);
typedef void (*pfv5)(alt_u32, alt_u32, alt_u32, alt_u32, alt_u32);
typedef int  (*pfi4)(alt_u32, alt_u32, alt_u32, alt_u32);
typedef int  (*pfi5)(alt_u32, alt_u32, alt_u32, alt_u32, alt_u32);

typedef struct {
	alt_u32 fnc;
	alt_u8  ret_prm;
}sFncTbl, *psFncTbl;

sFncTbl fncTbl[] = {
	{(alt_u32)writePixel, 0x03},
	{(alt_u32)writeLine , 0x05},
	{(alt_u32)drawRect  , 0x05},
	{(alt_u32)fillRect  , 0x05},
	{(alt_u32)drawCircle, 0x04},
	{(alt_u32)fillCircle, 0x04},
	{(alt_u32)drawChar  , 0x15},
	{(alt_u32)drawTxt   , 0x14},
};

#include "Fonts/FreeMono12pt7b.h"
GFXfont *gfxFont = (GFXfont*)&FreeMono12pt7b;

/**
 */
void gfxInit(int devs)
{
	int x;
	int i;
	char txt[] = {"Hello World"};

    memset(FB_BASE, 0, FB_WIDTH*FB_HEIGHT*2);

	writePixel(10, 10, 0x7C00);
	writePixel(10, 11, 0x03E0);
	writePixel(10, 12, 0x001F);

	writeLine( 0, 0, 10, 20, 0x7C00);
	writeLine(10, 0, 00, 20, 0x7C00);
	writeLine( 0, 0, 20, 10, 0x03E0);
	writeLine(20, 0, 00, 10, 0x03E0);

	drawRect(20, 20, 20, 20, 0x7C00);
	drawRect(40, 20, 20, 20, 0x03E0);
	drawRect(60, 20, 20, 20, 0x001F);

	fillRect(20, 40, 20, 20, 0x7C00);
	fillRect(40, 40, 20, 20, 0x03E0);
	fillRect(60, 40, 20, 20, 0x001F);

	drawCircle(100, 40, 15, 0x7C00);
	drawCircle(100, 40, 10, 0x03E0);
	drawCircle(100, 40,  5, 0x001F);

	fillCircle(150, 40, 15, 0x7C00);
	fillCircle(150, 40, 10, 0x03E0);
	fillCircle(150, 40,  5, 0x001F);

	x = 10;
	x+=drawChar(x, 100, 0x7C00, 2, 'A');
	x+=drawChar(x, 100, 0x7C00, 2, 'a');
	x+=drawChar(x, 100, 0x7C00, 2, 'B');
	x+=drawChar(x, 100, 0x7C00, 2, 'b');

	x = 10;
	for(i=0; txt[i]; i++){
		x += drawChar(x, 150, 0x03ff, 1, txt[i]);
	}


	writeLine(0x0100, 0x0100, 0x0140, 0x0140, 0xFFFF);
	drawRect(0x0100, 0x0140, 0x0040, 0x0040, 0xFFFF);
	fillRect(0x0150, 0x0140, 0x0040, 0x0040, 0xFFFF);
	drawTxt(50, 200, 0x7FFF, (alt_u8*)"Test draw text");

	alt_u32 volatile *rpc = (alt_u32*)DPRAM_BASE;

	rpc[0] = 1;
	rpc[1] = 0x0150;
	rpc[2] = 0x0100;
	rpc[3] = 0x0140;
	rpc[4] = 0x0140;
	rpc[5] = 0xFFFF;
	gfxCmd();

	rpc[0] = 7;
	rpc[1] = 20;
	rpc[2] = 100;
	rpc[3] = 0x7FE;
	rpc[4] = 0x30313233;
	rpc[5] = 0x34353637;
	rpc[6] = 0;
	gfxCmd();

}

/**
 *
 */
void gfxCmd(void)
{
	alt_u32 volatile *rpc = (alt_u32*)DPRAM_BASE;
	int ret = -1;

	if (rpc[0] >= sizeof(fncTbl)/sizeof(sFncTbl)) {
		return;
	}
	switch (fncTbl[rpc[0]].ret_prm) {
	case 0x03:
		((pfv3)fncTbl[rpc[0]].fnc)(rpc[1], rpc[2], rpc[3]);
		break;
	case 0x04:
		((pfv4)fncTbl[rpc[0]].fnc)(rpc[1], rpc[2], rpc[3], rpc[4]);
		break;
	case 0x05:
		((pfv5)fncTbl[rpc[0]].fnc)(rpc[1], rpc[2], rpc[3], rpc[4], rpc[5]);
		break;
	case 0x14:
		ret = ((pfi4)fncTbl[rpc[0]].fnc)(rpc[1], rpc[2], rpc[3], (alt_u8*)&rpc[4]);
		break;
	case 0x15:
		ret = ((pfi5)fncTbl[rpc[0]].fnc)(rpc[1], rpc[2], rpc[3], rpc[4], rpc[5]);
		break;
	}
	rpc[1] = ret;
}

/**
 * Draw a Point of color at x, y
 */
alt_u32 writePixel(alt_u16 x, alt_u16 y, uint16_t color)
{
	alt_u16 *p;

	p = FB_BASE + (x + y*FB_WIDTH);
	*p = color;
	return 0;
}

/**
 */
alt_u32 writeHLine(alt_u16 x, alt_u16 y, alt_u16 l, uint16_t color)
{
	alt_u32 i;

	for(i=x; i<x+l; i++){
		writePixel(i, y, color);
	}
	return 0;
}

/**
 */
alt_u32 writeVLine(alt_u16 x, alt_u16 y, alt_u16 l, alt_u16 color)
{
	alt_u32 i;

	for(i=y; i<y+l; i++){
		writePixel(x, i, color);
	}
	return 0;
}

/**
 *
 */
alt_u32 writeLine(alt_u16 x0, alt_u16 y0, alt_u16 x1, alt_u16 y1, alt_u16 color)
{
	alt_16 steep = abs(y1 - y0) > abs(x1 - x0);
	alt_16 dx, dy;
	alt_16 err;
	alt_16 ystep;

	if (steep) {
		_swap_int16_t(x0, y0);
		_swap_int16_t(x1, y1);
	}

	if (x0 > x1) {
		_swap_int16_t(x0, x1);
		_swap_int16_t(y0, y1);
	}

	dx = x1 - x0;
	dy = abs(y1 - y0);
	err = dx / 2;

	if(y0<y1){
		ystep = 1;
	} else {
		ystep = -1;
	}

	for(; x0<=x1; x0++){
		if(steep){
			writePixel(y0, x0, color);
		}else{
			writePixel(x0, y0, color);
		}
		err -= dy;
		if(err<0){
			y0 += ystep;
			err += dx;
		}
	}
	return 0;
}

/**
 * Draw a rectangle
 */
alt_u32 drawRect(alt_u16 x, alt_u16 y, alt_u16 w, alt_u16 h, alt_u16 color)
{
	writeHLine(x, y, w, color);
	writeHLine(x, y+h-1, w, color);
	writeVLine(x, y, h, color);
	writeVLine(x+w-1, y, h, color);
	return 0;
}
/**
 */
alt_u32 fillRect(alt_u16 x, alt_u16 y, alt_u16 w, alt_u16 h, alt_u16 color)
{
	alt_u32 i;
	for (i=x; i<x+w; i++) {
		writeVLine(i, y, h, color);
	}
	return 0;
}

/**
 *
 */
alt_u32 drawCircleHelper(alt_16 x0, alt_16 y0, alt_16 r, alt_u8 cornername, alt_u16 color)
{
	alt_16 f     = 1 - r;
	alt_16 ddF_x = 1;
	alt_16 ddF_y = -2 * r;
	alt_16 x     = 0;
	alt_16 y     = r;

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;
        if (cornername & 0x4) {
            writePixel(x0 + x, y0 + y, color);
            writePixel(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2) {
            writePixel(x0 + x, y0 - y, color);
            writePixel(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8) {
            writePixel(x0 - y, y0 + x, color);
            writePixel(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1) {
            writePixel(x0 - y, y0 - x, color);
            writePixel(x0 - x, y0 - y, color);
        }
    }
	return 0;
}

/**
 * Draw a circle outline
 */
alt_u32 drawCircle(alt_u16 x0, alt_u16 y0, alt_u16 r, alt_u16 color)
{
	alt_16 f     =  1 - r;
	alt_16 ddF_x =  1;
	alt_16 ddF_y = -2 * r;
	alt_16 x     =  0;
	alt_16 y     =  r;

	writePixel(x0  , y0+r, color);
	writePixel(x0  , y0-r, color);
	writePixel(x0+r, y0  , color);
	writePixel(x0-r, y0  , color);

	while(x<y){
		if(f>=0){
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		writePixel(x0 + x, y0 + y, color);
		writePixel(x0 - x, y0 + y, color);
		writePixel(x0 + x, y0 - y, color);
		writePixel(x0 - x, y0 - y, color);
		writePixel(x0 + y, y0 + x, color);
		writePixel(x0 - y, y0 + x, color);
		writePixel(x0 + y, y0 - x, color);
		writePixel(x0 - y, y0 - x, color);
	}
	return 0;
}

/**
 * Used to do circles and roundrects
 *
 */
alt_u32 fillCircleHelper(alt_u16 x0, alt_u16 y0, alt_u16 r, uint8_t cornername, alt_u16 delta, alt_u16 color)
{
	alt_16 f     =  1 - r;
	alt_16 ddF_x =  1;
	alt_16 ddF_y = -2 * r;
	alt_16 x     =  0;
	alt_16 y     =  r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f     += ddF_y;
		}
		x++;
		ddF_x += 2;
		f     += ddF_x;

		if (cornername & 0x1) {
			writeVLine(x0+x, y0-y, 2*y+1+delta, color);
			writeVLine(x0+y, y0-x, 2*x+1+delta, color);
		}
		if (cornername & 0x2) {
			writeVLine(x0-x, y0-y, 2*y+1+delta, color);
			writeVLine(x0-y, y0-x, 2*x+1+delta, color);
		}
	}
	return 0;
}

/**
 */
alt_u32 fillCircle(alt_u16 x0, alt_u16 y0, alt_u16 r, alt_u16 color)
{
	writeVLine(x0, y0-r, 2*r+1, color);
	fillCircleHelper(x0, y0, r, 3, 0, color);
	return 0;
}

/**
 * Draw a rounded rectangle
 */
alt_u32 drawRoundRect(int16_t x, int16_t y, int16_t w,
        int16_t h, int16_t r, uint16_t color) {
    // smarter version
    writeHLine(x+r  , y    , w-2*r, color); // Top
    writeHLine(x+r  , y+h-1, w-2*r, color); // Bottom
    writeVLine(x    , y+r  , h-2*r, color); // Left
    writeVLine(x+w-1, y+r  , h-2*r, color); // Right
    // draw four corners
    drawCircleHelper(x+r    , y+r    , r, 1, color);
    drawCircleHelper(x+w-r-1, y+r    , r, 2, color);
    drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
    drawCircleHelper(x+r    , y+h-r-1, r, 8, color);
	return 0;
}

/**
 * Fill a rounded rectangle
 */
alt_u32 fillRoundRect(int16_t x, int16_t y, int16_t w,
        int16_t h, int16_t r, uint16_t color) {
    // smarter version
    fillRect(x+r, y, w-2*r, h, color);

    // draw four corners
    fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
    fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1, color);
	return 0;
}

// TEXT- AND CHARACTER-HANDLING FUNCTIONS ----------------------------------

/**
 * Draw a character
 */
alt_u32 drawChar(alt_u16 x, alt_u16 y, alt_u16 color, uint8_t size, alt_u8 c)
{
	// Custom font

	// Character is assumed previously filtered by write() to eliminate
	// newlines, returns, non-printable characters, etc.  Calling
	// drawChar() directly with 'bad' characters of font may cause mayhem!

	c -= gfxFont->first;
	GFXglyph *glyph  = &(gfxFont->glyph[c]);
	alt_u8  *bitmap = gfxFont->bitmap;
	alt_u16  bo = glyph->bitmapOffset;
	alt_u8   w  = glyph->width;
	alt_u8   h  = glyph->height;
	alt_8    xo = glyph->xOffset;
	alt_8    yo = glyph->yOffset;
	alt_u8   xx, yy, bits = 0, bit = 0;
	alt_16   xo16 = 0, yo16 = 0;

	if(size > 1){
		xo16 = xo;
		yo16 = yo;
	}

	// Todo: Add character clipping here

	// NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
	// THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
	// has typically been used with the 'classic' font to overwrite old
	// screen contents with new data.  This ONLY works because the
	// characters are a uniform size; it's not a sensible thing to do with
	// proportionally-spaced fonts with glyphs of varying sizes (and that
	// may overlap).  To replace previously-drawn text when using a custom
	// font, use the getTextBounds() function to determine the smallest
	// rectangle encompassing a string, erase the area with fillRect(),
	// then draw new text.  This WILL infortunately 'blink' the text, but
	// is unavoidable.  Drawing 'background' pixels will NOT fix this,
	// only creates a new set of problems.  Have an idea to work around
	// this (a canvas object type for MCUs that can afford the RAM and
	// displays supporting setAddrWindow() and pushColors()), but haven't
	// implemented this yet.


	for(yy=0; yy<h; yy++) {
		for(xx=0; xx<w; xx++) {
			if(!(bit++ & 7)) {
				bits = bitmap[bo++];
			}
			if(bits & 0x80) {
				if(size == 1) {
					writePixel(x+xo+xx, y+yo+yy, color);
				} else {
					fillRect(x+(xo16+xx)*size, y+(yo16+yy)*size, size, size, color);
				}
			}
			bits <<= 1;
		}
	}
	return glyph->xAdvance * size;
}

/**
 *
 */
alt_u32 drawTxt(alt_u16 x, alt_u16 y, alt_u16 color, alt_u8* txt)
{
	int size;
	int i;

	size = 0;
	for(i=0; txt[i]; i++){
		size += drawChar(x+size, y, color, 1, txt[i]);
	}

	return size;
}
