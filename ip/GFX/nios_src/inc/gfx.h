/*
 * gfx.h
 *
 *  Created on: Apr 11, 2018
 *      Author: max
 */



#ifndef GFX_H_
#define GFX_H_

#include <alt_types.h>

#define GFX_UID 0x84B11
#define GFX_IP_VER   0x0000
#define GFX_DRV_VER  0x0004
#define GFX_VER (((GFX_IP_VER)<<16)|(GFX_DRV_VER))

#define GFX_FB_WIDTH   640
#define GFX_FB_HEIGHT  480
#define GFX_FB_BASE    0x00000000

#define GFX_FONTS 1
#define GFX_FONT_FILE   "Org_01.h"
#define GFX_FONT_NAME   Org_01

#define GFX_GC_FMT_ARGB16 1
#define GFX_GC_FMT_XGRB32 2

#define GFX_GC_ROT90    0x00000001

#if defined(GFX_FONTS) && (GFX_FONTS == 1)

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

#endif /* defined(GFX_FONTS) && (GFX_FONTS == 1) */

typedef struct {
  alt_u32   width;
  alt_u32   height;
  alt_u32   stride;
  alt_u16   bpp;
  alt_u16   fmt;
  alt_u32   color;
  void     *fb;
  alt_u32 (*pix)(void* pGc, alt_u16 x, alt_u16 y);
  alt_u32   flg;
#if defined(GFX_FONTS) && (GFX_FONTS == 1)
  GFXfont  *pFnt;
  alt_u32   txtColor;
  alt_u16   cursor_x;
  alt_u16   cursor_y;
  alt_u32   size;
#endif /* defined(GFX_FONTS) && (GFX_FONTS == 1) */
} GFXgc;

typedef struct {
  alt_u32  type;
  alt_u16  width;
  alt_u16  height;    // Bitmap dimensions in pixels
  alt_u32  size;
  alt_u16 *bmp;       // pointer to bitmap
} GFXbmp;

void gfxInit(int devs);
void gfxRpc(void);

alt_u32 wp16(void* pGc, alt_u16 x, alt_u16 y);
alt_u32 wp32(void* pGc, alt_u16 x, alt_u16 y);

// TODO togliere alt_u32 gcSet(GFXgc* pGc);

alt_u32 writePixel       (GFXgc* pGc, alt_u16 x, alt_u16 y, alt_u32 color);
alt_u32 writeLine        (GFXgc* pGc, alt_u16 x0, alt_u16 y0, alt_u16 x1, alt_u16 y1, alt_u32 color);
alt_u32 drawRect         (GFXgc* pGc, alt_u16 x, alt_u16 y, alt_u16 w, alt_u16 h, alt_u32 color);
alt_u32 fillRect         (GFXgc* pGc, alt_u16 x, alt_u16 y, alt_u16 w, alt_u16 h, alt_u32 color);
alt_u32 drawCircle       (GFXgc* pGc, alt_u16 x, alt_u16 y, alt_u16 r, alt_u32 color);
alt_u32 fillCircle       (GFXgc* pGc, alt_u16 x, alt_u16 y, alt_u16 r, alt_u32 color);
#if defined(GFX_FONTS) && (GFX_FONTS == 1)
alt_u32 drawChar         (GFXgc* pGc, alt_u16 x, alt_u16 y, alt_u32 color, alt_u8 size, alt_u8 c);
alt_u32 drawTxt          (GFXgc* pGc, alt_u16 x, alt_u16 y, alt_u32 color, alt_u8* txt);
alt_u32 setFont          (GFXgc* pGc, alt_u32 num);
alt_u32 setCursor        (GFXgc* pGc, alt_u32 x, alt_u32 y);
alt_u32 setTextSize      (GFXgc* pGc, alt_u16 size);
alt_u32 setColor         (GFXgc* pGc, alt_u32 color);
alt_u32 setAlpha         (GFXgc* pGc, alt_u8 alpha);
alt_u32 drawCharAtCursor (GFXgc* pGc, alt_u8 c);
#endif /* defined(GFX_FONTS) && (GFX_FONTS == 1) */
alt_u32 drawBmp          (GFXgc* pGc, GFXbmp* bmp, alt_u16 x, alt_u16 y, alt_u32 color);

#endif /* GFX_H_ */
