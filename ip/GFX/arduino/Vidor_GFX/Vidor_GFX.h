/*
  This file is part of the VidorGraphics library.
  Copyright (c) 2018 Arduino SA. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _VIDOR_GFX_H
#define _VIDOR_GFX_H

#include <Arduino.h>
#include "VidorUtils.h"
#include "VidorIP.h"
#include "defines.h"
#include "Vidor_NeoPixel.h"

#define VIDOR_WIDTH  640
#define VIDOR_HEIGHT 480

class Vidor_GFX;

class Vidor_GFXtext : public Print {
  public:
    Vidor_GFXtext(Vidor_GFX* parent) {
      this->parent = parent;
    }
    void setColor(uint32_t color);
    void setAlpha(uint8_t alpha=0xff) {/*deprecated*/};
    void setFont(uint32_t idx);
    void setSize(uint16_t size);
    void setCursor(uint16_t x,uint16_t y);
    virtual size_t write(uint8_t c);
    virtual size_t write(char* string, int len);
  private:
    uint32_t color;
    uint16_t x, y, size;
    Vidor_GFX* parent;
};

class Vidor_GFX : public Print, public VidorIP {
  public:
    Vidor_GFX();
    Vidor_GFX(Vidor_NeoPixel& instance);
    int begin();
    void drawPixel(uint16_t x, uint16_t y, uint32_t color, uint8_t alpha=0xff);
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t    color, uint8_t alpha=0xff);
    void drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color, uint8_t alpha=0xff);
    void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color, uint8_t alpha=0xff);
    void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint32_t color, uint8_t alpha=0xff);
    void fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint32_t color, uint8_t alpha=0xff);
    void drawChar(uint16_t x, uint16_t y, uint32_t color, uint8_t size,unsigned char c, uint8_t alpha=0xff);
    //void setFont(uint32_t num);
    virtual size_t write(uint8_t c);
    using Print::write;
    uint32_t Color(uint8_t r, uint8_t g, uint8_t b);
    uint32_t Red();
    uint32_t Green();
    uint32_t Blue();
    uint32_t Yellow();
    uint32_t lightBlue();
    uint32_t Purple();
    uint32_t Black();
    uint32_t Brown();
    uint32_t White();
    void Cross(uint16_t x, uint16_t y, uint32_t c, uint8_t alpha=0xff);
    Vidor_GFXtext text = Vidor_GFXtext(this);
    void* framebuffer;

  friend class Vidor_CAM;
  friend class Vidor_GFXbuffer;
};

#define NP_SEQ_FLG_START      0x00000001
#define NP_SEQ_FLG_STOP       0x00000002
#define NP_SEQ_FLG_SINGLE     0x00000004
#define NP_SEQ_FLG_LOOP       0x00000008
#define NP_SEQ_FLG_BUF_LOOP   0x00000010
#define NP_SEQ_FLG_INV_LOOP   0x00000020

#define GFX_GC_ROT90    0x00000001
#define GFX_GC_ROT270   0x00000002
#define GFX_GC_FLIP_H   0x00000004
#define GFX_GC_FLIP_V   0x00000008

typedef enum ScrollDirection {
  LEFT_TO_RIGHT = 0,
  RIGHT_TO_LEFT
};

class Vidor_GFXbuffer {
  public:
    Vidor_GFXbuffer(Vidor_GFX& gfx, int x, int y, bool zigzag) {
      np = (Vidor_NeoPixel*)gfx.framebuffer;
      np->initialized = true;
      np->bufLen = x * y;
      this->gfx = &gfx;
      this->x = x;
      this->y = y;
      this->zigzag = zigzag;
    }
    void scroll(int delay = 100, ScrollDirection direction = LEFT_TO_RIGHT);
    void noScroll();
    void begin(bool rotate90 = false, bool mirrorX = false, bool mirrorY = false);
  private:
    int x;
    int y;
    bool zigzag;
    Vidor_NeoPixel* np;
    Vidor_GFX* gfx;
    void privateScroll(int flags, int delay);
  protected:
    friend class Vidor_GFX;
    friend class Vidor_NeoPixel;
};


#endif // _VIDOR_GFX_H
