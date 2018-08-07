/*
 * gfx.h
 *
 *  Created on: Apr 11, 2018
 *      Author: max
 */

#ifndef GFX_H_
#define GFX_H_

#include <alt_types.h>

#define GFX_GC_FMT_ARGB16 1
#define GFX_GC_FMT_XGRB32 2

#define GFX_GC_ROT90    0x00000001

typedef struct {
  alt_u32   width;
  alt_u32   height;
  alt_u32   stride;
  alt_u16   bpp;
  alt_u16   fmt;
  void     *fb;
  alt_u32 (*wp)(alt_u16 x, alt_u16 y, alt_u32 color);
  alt_u32   flg;
} GFXgc;

typedef struct {
  alt_u32  type;
  alt_u16  width;
  alt_u16  height;    // Bitmap dimensions in pixels
  alt_u32  size;
  alt_u16 *bmp;       // pointer to bitmap
} GFXbmp;

void gfxInit(int devs);
void gfxCmd(void);

alt_u32 gcSet(GFXgc* pGc);

alt_u32 writePixel(alt_u16 x, alt_u16 y, alt_u32 color);
alt_u32 writeLine (alt_u16 x0, alt_u16 y0, alt_u16 x1, alt_u16 y1, alt_u32 color);
alt_u32 drawRect  (alt_u16 x, alt_u16 y, alt_u16 w, alt_u16 h, alt_u32 color);
alt_u32 fillRect  (alt_u16 x, alt_u16 y, alt_u16 w, alt_u16 h, alt_u32 color);
alt_u32 drawCircle(alt_u16 x0, alt_u16 y0, alt_u16 r, alt_u32 color);
alt_u32 fillCircle(alt_u16 x0, alt_u16 y0, alt_u16 r, alt_u32 color);
alt_u32 drawChar  (alt_u16 x, alt_u16 y, alt_u32 color, alt_u8 size, alt_u8 c);
alt_u32 drawTxt   (alt_u16 x, alt_u16 y, alt_u32 color, alt_u8* txt);
alt_u32 drawBmp   (GFXbmp* bmp, alt_u16 x, alt_u16 y, alt_u32 color);

#endif /* GFX_H_ */
