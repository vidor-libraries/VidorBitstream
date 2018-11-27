/**
 * Copyright 2018 ARDUINO SA (http://www.arduino.cc/)
 * This file is part of Vidor IP.
 * Copyright (c) 2018
 * Authors: Massimiliano Agneni
 *
 * This software is released under:
 * The GNU General Public License, which covers the main part of Vidor IP
 * The terms of this license can be found at:
 * https://www.gnu.org/licenses/gpl-3.0.en.html
 *
 * You can be released from the requirements of the above licenses by purchasing
 * a commercial license. Buying such a license is mandatory if you want to modify or
 * otherwise use the software for commercial activities involving the Arduino
 * software without disclosing the source code of your own applications. To purchase
 * a commercial license, send an email to license@arduino.cc.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <alt_types.h>

#include "config.h"
#include "mb.h"
#include "np.h"
#include "gfx.h"

#define _swap_int16_t(a,b) {int16_t t=a; a=b; b=t;}

#if defined(GFX_FONTS) && (GFX_FONTS == 1)
  #include GFX_FONT_FILE
#endif /* defined(GFX_FONTS) && (GFX_FONTS == 1) */

#if defined(GFX_LOGO) && (GFX_LOGO == 1)
  #include GFX_LOGO_FILE
#endif /* defined(GFX_LOGO) && (GFX_LOGO == 1) */

#if defined(NP_GFX) && (NP_GFX == 1)
  #define GFX_GC_NUM (2+NEOPIXEL_0_CHANNELS)
#else
  #define GFX_GC_NUM 2
#endif

#define SEC_RAM  __attribute__((__section__(".rwdata")))

/**
 * local variables
 */

/**
 * Default graphic context (FrameBuffer)
 */
GFXgc gfxDefaultGc = {
  GFX_FB_WIDTH,
  GFX_FB_HEIGHT,
  1,
  16,
  GFX_GC_FMT_ARGB16,
  0xffffffff,
  (void*)0,
  wp16,
  rd16,
  0,
#if defined(GFX_FONTS) && (GFX_FONTS == 1)
  (GFXfont*)&GFX_FONT_NAME,
  0xffffffff,
  0,
  0,
  1,
#endif /* defined(GFX_FONTS) && (GFX_FONTS == 1) */
};

/**
 * Camera graphic context
 */
GFXgc gfxCameraGc = {
  GFX_FB_WIDTH,
  GFX_FB_HEIGHT,
  1,
  16,
  GFX_GC_FMT_ARGB16,
  0xffffffff,
  (void*)0,
  wp16,
  rd16, //RDP read pixel
  0,
#if defined(GFX_FONTS) && (GFX_FONTS == 1)
  NULL,
  0,
  0,
  0,
  0,
#endif /* defined(GFX_FONTS) && (GFX_FONTS == 1) */
};

/**
 * array of defined graphic contexts
 */
GFXgc *pGfxGc[GFX_GC_NUM] = {
  &gfxDefaultGc,
  &gfxCameraGc,
};

alt_u32 gfxSetup(alt_u32 cmd);
alt_u32 gfxEnd(alt_u32 cmd);

/**
 *
 */
void gfxRpc(void)
{
  alt_u32 volatile *rpc = mbPtrGet();
  alt_u32           ret;
  GFXgc            *pGc;

  ret = -1;
  if ((fpgaIp[RPC_GIID(rpc[0])].disc & 0xFFFFF) != GFX_UID) {
    rpc[1] = ret;
    return ;
  }

  if (RPC_CHN(rpc[0]) >= GFX_GC_NUM) {
    rpc[1] = -1;
    return;
  }
  pGc = pGfxGc[RPC_CHN(rpc[0])];

  ret = -1;
  switch (RPC_PID(rpc[0])) {
  case 2: ret = gfxSetup(rpc[0]); break;
  case 4: ret = gfxEnd(rpc[0]); break;

  case 5:
    ret = writePixel(pGc, rpc[1], rpc[2], rpc[3]);
    break;
  case 6:
    ret = writeLine(pGc, rpc[1], rpc[2], rpc[3], rpc[4], rpc[5]);
    break;
  case 7:
    ret = drawRect(pGc, rpc[1], rpc[2], rpc[3], rpc[4], rpc[5]);
    break;
  case 8:
    ret = fillRect(pGc, rpc[1], rpc[2], rpc[3], rpc[4], rpc[5]);
    break;
  case 9:
    ret = drawCircle(pGc, rpc[1], rpc[2], rpc[3], rpc[4]);
    break;
  case 10:
    ret = fillCircle(pGc, rpc[1], rpc[2], rpc[3], rpc[4]);
    break;
#if defined(GFX_FONTS) && (GFX_FONTS == 1)
  case 11:
    ret = drawChar(pGc, rpc[1], rpc[2], rpc[3], rpc[4], rpc[5]);
    break;
  case 12:
    ret = drawTxt(pGc, rpc[1], rpc[2], rpc[3], (alt_u8*)&rpc[4]);
    break;
  case 13:
    ret = setFont(pGc, rpc[1]);
    break;
  case 14:
    ret = setCursor(pGc, rpc[1],rpc[2]);
    break;
  case 15:
    ret = setTextSize(pGc, rpc[1]);
    break;
  case 16:
    ret = setColor(pGc, rpc[1]);
    break;
  case 17:
    ret = setAlpha(pGc, rpc[1]);
    break;
  case 18:
    ret = drawCharAtCursor(pGc, rpc[1]);
    break;
#endif /* defined(GFX_FONTS) && (GFX_FONTS == 1) */
  }
  rpc[1] = ret;
}

/**
 *
 */
alt_u32 gfxSetup(alt_u32 cmd)
{
  alt_u8    giid = RPC_GIID(cmd);

  gfxDefaultGc.fb = (void*)(((psGfxPriv)(fpgaIp[giid].priv))->fb_base  | 0x80000000);
  gfxCameraGc.fb  = (void*)(((psGfxPriv)(fpgaIp[giid].priv))->cam_base | 0x80000000);

#if defined(NP_GFX) && (NP_GFX == 1)
  int i;
  for (i=0; i<NEOPIXEL_0_CHANNELS; i++) {
    pGfxGc[2+i] = &npGfxGc[i];
  }
#endif

  memset(gfxDefaultGc.fb, 0xFF, GFX_FB_WIDTH*GFX_FB_HEIGHT*2);

#if defined(GFX_LOGO) && (GFX_LOGO == 1)
  drawBmp(&gfxDefaultGc, (GFXbmp*)&GFX_LOGO_NAME,
          (GFX_FB_WIDTH-GFX_LOGO_NAME.width)/2,
          (GFX_FB_HEIGHT-GFX_LOGO_NAME.height)/2,
          33396);
#endif

  return 0;
}

/**
 *
 */
alt_u32 gfxEnd(alt_u32 cmd)
{
  return 0;
}

/**
 * convert color
 */
alt_u32 SEC_RAM colorFormat(GFXgc* pGc, alt_u32 color)
{
  switch (pGc->fmt) {
  case GFX_GC_FMT_ARGB16:
    color = ((color&0x000000F8)>> 3)|
            ((color&0x0000F800)>> 6)|
            ((color&0x00F80000)>> 9)|
            ((color&0x80000000)>>16);
    break;
  case GFX_GC_FMT_XGRB32:
    color = ((color&0x000000FF)   )|
            ((color&0x0000FF00)<<8)|
            ((color&0x00FF0000)>>8);
    break;
  }
  return color;
}

/**
 */
void SEC_RAM xy_convert(GFXgc* pGc, alt_u16 *x, alt_u16 *y)
{
  if (pGc->flg & GFX_GC_ROT90) {
    alt_u16 t;
    t = *x;
    if (pGc->flg & GFX_GC_FLIP_H) {
      *x = pGc->width - *y - 1;
    } else {
      *x = *y;
    }
    if (pGc->flg & GFX_GC_FLIP_V) {
      *y = pGc->height - t - 1;
    }else{
      *y = t;
    }
  } else {
    if (pGc->flg & GFX_GC_FLIP_H) {
      *x = pGc->width - *x - 1;
    }
    if (pGc->flg & GFX_GC_FLIP_V) {
      *y = pGc->height - *y - 1;
    }
  }
}

/**
 * Draw a Point of color at x, y
 */
alt_u32 SEC_RAM writePixel(GFXgc* pGc, alt_u16 x, alt_u16 y, alt_u32 color)
{
  color = colorFormat(pGc, color);

  xy_convert(pGc, &x, &y);

  if (x>=pGc->width || y>=pGc->height) {
    return -1;
  }

  if (pGc->bpp == 16) {
    alt_u16 *p;

    p = (alt_u16*)pGc->fb + ((x + y*pGc->width) * pGc->stride);
    *p = color;
  } else {
    alt_u32 *p;

    p = (alt_u32*)pGc->fb + ((x + y*pGc->width) * pGc->stride);
    *p = color;
  }
  return 0;
}

/**
 */
alt_u32 SEC_RAM wp16(void* arg, alt_u16 x, alt_u16 y)
{
  GFXgc   *pGc = (GFXgc*)arg;
  alt_u16 *p;

  xy_convert(pGc, &x, &y);
  p = (alt_u16*)pGc->fb + ((x + y*pGc->width) * pGc->stride);
  *p = (alt_u16)pGc->color;
  return 0;
}

/**
 */
alt_u32 SEC_RAM rd16(void* arg, alt_u16 x, alt_u16 y)
{
  GFXgc   *pGc = (GFXgc*)arg;
  alt_u16 *p;

  p = (alt_u16*)pGc->fb + ((x + y*pGc->width) * pGc->stride);
  return *p;
}

/**
 */
alt_u32 SEC_RAM wp32(void* arg, alt_u16 x, alt_u16 y)
{
  GFXgc   *pGc = (GFXgc*)arg;
  alt_u32 *p;

  xy_convert(pGc, &x, &y);
  p = (alt_u32*)pGc->fb + ((x + y*pGc->width) * pGc->stride);
  *p = pGc->color;
  return 0;
}

/**
 */
alt_u32 SEC_RAM rd32(void* arg, alt_u16 x, alt_u16 y)
{
  GFXgc   *pGc = (GFXgc*)arg;
  alt_u32 *p;

  p = (alt_u32*)pGc->fb + ((x + y*pGc->width) * pGc->stride);
  return *p;
}

/**
 */
alt_u32 writeHLine(GFXgc* pGc, alt_u16 x, alt_u16 y, alt_u16 l, alt_u32 color)
{
  alt_u32 i;

  pGc->color = colorFormat(pGc, color);
  for(i=x; i<x+l; i++){
    pGc->pix(pGc, i, y);
  }
  return 0;
}

/**
 */
alt_u32 writeVLine(GFXgc* pGc, alt_u16 x, alt_u16 y, alt_u16 l, alt_u32 color)
{
  alt_u32 i;

  pGc->color = colorFormat(pGc, color);
  for(i=y; i<y+l; i++){
    pGc->pix(pGc, x, i);
  }
  return 0;
}

/**
 *
 */
alt_u32 writeLine(GFXgc* pGc, alt_u16 x0, alt_u16 y0, alt_u16 x1, alt_u16 y1, alt_u32 color)
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

  pGc->color = colorFormat(pGc, color);
  for(; x0<=x1; x0++){
    if(steep){
      pGc->pix(pGc, y0, x0);
    }else{
      pGc->pix(pGc, x0, y0);
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
alt_u32 drawRect(GFXgc* pGc, alt_u16 x, alt_u16 y, alt_u16 w, alt_u16 h, alt_u32 color)
{
  writeHLine(pGc, x, y, w, color);
  writeHLine(pGc, x, y+h-1, w, color);
  writeVLine(pGc, x, y, h, color);
  writeVLine(pGc, x+w-1, y, h, color);
  return 0;
}

/**
 * Draw fille rectangle
 */
alt_u32 fillRect(GFXgc* pGc, alt_u16 x, alt_u16 y, alt_u16 w, alt_u16 h, alt_u32 color)
{
  alt_u32 i;
  for (i=x; i<x+w; i++) {
    writeVLine(pGc, i, y, h, color);
  }
  return 0;
}

/**
 *
 */
alt_u32 drawCircleHelper(GFXgc* pGc, alt_16 x0, alt_16 y0, alt_16 r, alt_u8 cornername, alt_u32 color)
{
  alt_16 f     = 1 - r;
  alt_16 ddF_x = 1;
  alt_16 ddF_y = -2 * r;
  alt_16 x     = 0;
  alt_16 y     = r;

  pGc->color = colorFormat(pGc, color);

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
      pGc->pix(pGc, x0 + x, y0 + y);
      pGc->pix(pGc, x0 + y, y0 + x);
    }
    if (cornername & 0x2) {
      pGc->pix(pGc, x0 + x, y0 - y);
      pGc->pix(pGc, x0 + y, y0 - x);
    }
    if (cornername & 0x8) {
      pGc->pix(pGc, x0 - y, y0 + x);
      pGc->pix(pGc, x0 - x, y0 + y);
    }
    if (cornername & 0x1) {
      pGc->pix(pGc, x0 - y, y0 - x);
      pGc->pix(pGc, x0 - x, y0 - y);
    }
  }
  return 0;
}

/**
 * Draw a circle outline
 */
alt_u32 drawCircle(GFXgc* pGc, alt_u16 x0, alt_u16 y0, alt_u16 r, alt_u32 color)
{
  alt_16 f     =  1 - r;
  alt_16 ddF_x =  1;
  alt_16 ddF_y = -2 * r;
  alt_16 x     =  0;
  alt_16 y     =  r;

  pGc->color = colorFormat(pGc, color);

  pGc->pix(pGc, x0  , y0+r);
  pGc->pix(pGc, x0  , y0-r);
  pGc->pix(pGc, x0+r, y0  );
  pGc->pix(pGc, x0-r, y0  );

  while(x<y){
    if(f>=0){
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    pGc->pix(pGc, x0 + x, y0 + y);
    pGc->pix(pGc, x0 - x, y0 + y);
    pGc->pix(pGc, x0 + x, y0 - y);
    pGc->pix(pGc, x0 - x, y0 - y);
    pGc->pix(pGc, x0 + y, y0 + x);
    pGc->pix(pGc, x0 - y, y0 + x);
    pGc->pix(pGc, x0 + y, y0 - x);
    pGc->pix(pGc, x0 - y, y0 - x);
  }
  return 0;
}

/**
 * Used to do circles and roundrects
 *
 */
alt_u32 fillCircleHelper(GFXgc* pGc, alt_u16 x0, alt_u16 y0, alt_u16 r, alt_u8 cornername, alt_u16 delta, alt_u32 color)
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
      writeVLine(pGc, x0+x, y0-y, 2*y+1+delta, color);
      writeVLine(pGc, x0+y, y0-x, 2*x+1+delta, color);
    }
    if (cornername & 0x2) {
      writeVLine(pGc, x0-x, y0-y, 2*y+1+delta, color);
      writeVLine(pGc, x0-y, y0-x, 2*x+1+delta, color);
    }
  }
  return 0;
}

/**
 */
alt_u32 fillCircle(GFXgc* pGc, alt_u16 x0, alt_u16 y0, alt_u16 r, alt_u32 color)
{
  writeVLine(pGc, x0, y0-r, 2*r+1, color);
  fillCircleHelper(pGc, x0, y0, r, 3, 0, color);
  return 0;
}

/**
 * Draw a rounded rectangle
 */
alt_u32 drawRoundRect(GFXgc* pGc, alt_u16 x, alt_u16 y, alt_u16 w,
  alt_u16 h, alt_u16 r, alt_u32 color)
{
  // smarter version
  writeHLine(pGc, x+r  , y    , w-2*r, color); // Top
  writeHLine(pGc, x+r  , y+h-1, w-2*r, color); // Bottom
  writeVLine(pGc, x    , y+r  , h-2*r, color); // Left
  writeVLine(pGc, x+w-1, y+r  , h-2*r, color); // Right
  // draw four corners
  drawCircleHelper(pGc, x+r    , y+r    , r, 1, color);
  drawCircleHelper(pGc, x+w-r-1, y+r    , r, 2, color);
  drawCircleHelper(pGc, x+w-r-1, y+h-r-1, r, 4, color);
  drawCircleHelper(pGc, x+r    , y+h-r-1, r, 8, color);
  return 0;
}

/**
 * Fill a rounded rectangle
 */
alt_u32 fillRoundRect(GFXgc* pGc, alt_u16 x, alt_u16 y, alt_u16 w,
        alt_u16 h, alt_u16 r, alt_u32 color)
{
  // smarter version
  fillRect(pGc, x+r, y, w-2*r, h, color);

  // draw four corners
  fillCircleHelper(pGc, x+w-r-1, y+r, r, 1, h-2*r-1, color);
  fillCircleHelper(pGc, x+r    , y+r, r, 2, h-2*r-1, color);
  return 0;
}

#if defined(GFX_FONTS) && (GFX_FONTS == 1)

// TEXT- AND CHARACTER-HANDLING FUNCTIONS ----------------------------------

/**
 * Set text cursors
 */
alt_u32 setCursor(GFXgc* pGc, alt_u32 x, alt_u32 y)
{
  pGc->cursor_y=y;
  pGc->cursor_x=x;
  return 0;
}

/**
 * Set text size
 */
alt_u32 setTextSize(GFXgc* pGc, alt_u16 size)
{
  pGc->size = size;
  return 0;
}

/**
 * Set graphic context color
 */
alt_u32 setColor(GFXgc* pGc, alt_u32 color)
{
  pGc->txtColor = (pGc->txtColor & 0xff000000) | (color & 0xffffff);
  return 0;
}

/**
 * Set graphic context calpha
 */
alt_u32 setAlpha(GFXgc* pGc, alt_u8 alpha)
{
  pGc->txtColor = (pGc->txtColor & 0xffffff) | ((alt_u32)alpha << 24);
  return 0;
}

/**
 * Draw a character
 */
alt_u32 drawCharAtCursor(GFXgc* pGc, alt_u8 c)
{
  if (!pGc->pFnt) {
    return -1;
  }
  if(c=='\n') {
    pGc->cursor_y += pGc->size * pGc->pFnt->yAdvance;
    return 0;
  } else if (c=='\r') {
    pGc->cursor_x = 0;
    return 0;
  } else {
    pGc->cursor_x += drawChar(pGc, pGc->cursor_x, pGc->cursor_y, pGc->txtColor, pGc->size, c);
    return 0;
  }
}

/**
 * Draw a character
 */
alt_u32 drawChar(GFXgc* pGc, alt_u16 x, alt_u16 y, alt_u32 color, alt_u8 size, alt_u8 c)
{
  if (!pGc->pFnt) {
    return -1;
  }
  // Custom font

  // Character is assumed previously filtered by write() to eliminate
  // newlines, returns, non-printable characters, etc.  Calling
  // drawChar() directly with 'bad' characters of font may cause mayhem!

  c -= pGc->pFnt->first;
  GFXglyph *glyph  = &(pGc->pFnt->glyph[c]);
  alt_u8  *bitmap = pGc->pFnt->bitmap;
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
  pGc->color = colorFormat(pGc, color);
  for(yy=0; yy<h; yy++) {
    for(xx=0; xx<w; xx++) {
      if(!(bit++ & 7)) {
        bits = bitmap[bo++];
      }
      if(bits & 0x80) {
        if(size == 1) {
          pGc->pix(pGc, x+xo+xx, y+yo+yy);
        } else {
          fillRect(pGc, x+(xo16+xx)*size, y+(yo16+yy)*size, size, size, color);
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
alt_u32 drawTxt(GFXgc* pGc, alt_u16 x, alt_u16 y, alt_u32 color, alt_u8* txt)
{
  int size;
  int i;

  size = 0;
  for(i=0; txt[i]; i++){
    size += drawChar(pGc, x+size, y, color, 1, txt[i]);
  }
  return size;
}

/**
 */
alt_u32 setFont(GFXgc* pGc, alt_u32 num)
{
#ifdef GFX_NUM_FONTS
  if (num < GFX_NUM_FONTS) {
    pGc->pFnt = gfxFontRepo[num];
    return 0;
  }
#else
  pGc->pFnt = (GFXfont*)&GFX_FONT_NAME;
  return 0;
#endif
  return -1;
}

#endif /* defined(GFX_FONTS) && (GFX_FONTS == 1) */

/**
 *
 */
alt_u32 drawBmp(GFXgc* pGc, GFXbmp* bmp, alt_u16 x, alt_u16 y, alt_u32 color)
{
  int xpos = x;
  int i;
  int j;
  alt_u16 tmp;

  pGc->color = colorFormat(pGc, color);
  for(i=0; i<bmp->size; i++) {
    tmp = bmp->bmp[i];
    for(j=0; j<16; j++){
      if(tmp & 0x8000) {
        pGc->pix(pGc, xpos, y);
      }
      tmp <<= 1;
      xpos++;
      if(xpos == x+bmp->width) {
        xpos = x;
        y++;
      }
    }
  }
  return 0;
}
