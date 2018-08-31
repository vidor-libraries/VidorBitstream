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
#include "defines.h"
#include "Vidor_NeoPixel.h"

#define VIDOR_WIDTH  640
#define VIDOR_HEIGHT 480

#define GFX_PX    0
#define GFX_LN    1
#define GFX_DR    2
#define GFX_FR    3
#define GFX_DC    4
#define GFX_FC    5
//#define GFX_WC    6
#define GFX_WT    7
#define GFX_SF    8
#define GFX_CR    9
#define GFX_TS   10
#define GFX_SC   11
#define GFX_SA   12
#define GFX_WC   13

class Vidor_GFXtext {
  public:
    Vidor_GFXtext(int index=0);
    void setColor(uint32_t color);
    void setAlpha(uint8_t alpha=0xff);
    void setSize(uint16_t size);
    void setCursor(uint16_t x,uint16_t y);
  private:
    int idx;
    uint8_t devIdx;
  protected:
    friend class Vidor_GFX;
};

class Vidor_GFX : public Print {
  public:
    Vidor_GFX(int index=0);
    Vidor_GFX(Vidor_NeoPixel& instance);
    void drawPixel(uint16_t x, uint16_t y, uint32_t color, uint8_t alpha=0xff);
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t    color, uint8_t alpha=0xff);
    void drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color, uint8_t alpha=0xff);
    void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color, uint8_t alpha=0xff);
    void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint32_t color, uint8_t alpha=0xff);
    void fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint32_t color, uint8_t alpha=0xff);
    void drawChar(uint16_t x, uint16_t y, uint32_t color, uint8_t size,unsigned char c, uint8_t alpha=0xff);
    void setFont(uint32_t num);
    int getIndex() {
      return idx;
    }
    virtual size_t write(uint8_t c);
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
    Vidor_GFXtext text;
  private:
    uint8_t devIdx;
  protected:
    int idx;

  friend class Vidor_CAM;
  friend class Vidor_GFXbuffer;
};


#define NP_SEQ_FLG_START      0x00000001
#define NP_SEQ_FLG_STOP       0x00000002
#define NP_SEQ_FLG_SINGLE     0x00000004
#define NP_SEQ_FLG_LOOP       0x00000008
#define NP_SEQ_FLG_BUF_LOOP   0x00000010
#define NP_SEQ_FLG_INV_LOOP   0x00000020

class Vidor_GFXbuffer {
  public:
    Vidor_GFXbuffer(Vidor_NeoPixel& np, Vidor_GFX& gfx, int x, int y, bool zigzag) {
      devIdx = np.devIdx;
      np.init = true;
      idx = gfx.idx;
      this->x = x;
      this->y = y;
      this->zigzag = zigzag;
      this->np = &np;
    }
    void scroll(int delay = 100);
    void noScroll();
    void begin();
  private:
    int idx;
    uint8_t devIdx;
    int x;
    int y;
    bool zigzag;
    bool setup = false;
    Vidor_NeoPixel* np;
    void select();
    void privateScroll(int flags, int delay);
  protected:
    friend class Vidor_GFX;
    friend class Vidor_NeoPixel;
};


#endif // _VIDOR_GFX_H
