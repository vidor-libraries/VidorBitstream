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

#include "VidorMailbox.h"

#include "Vidor_GFX.h"

Vidor_GFX::Vidor_GFX(void) {
  text = Vidor_GFXtext();
}

Vidor_GFXtext::Vidor_GFXtext(void) {
}

void Vidor_GFX::drawPixel(uint16_t x, uint16_t y, uint16_t color, uint16_t alpha) {
  uint32_t rpc[5];
  if(x<=VIDOR_WEIGHT && y<=VIDOR_LENGTH && alpha<=1) {
    rpc[0] = MB_DEV_GFX | GFX_PX;
    rpc[1] = x;
    rpc[2] = y;
    rpc[3] = (color&0x7FFF)|(alpha<<15);
    VidorMailbox.sendCommand(rpc, 4);
  }
}

void Vidor_GFX::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color, uint16_t alpha) {
  uint32_t rpc[7];
  if(x0<=VIDOR_WEIGHT && y0<=VIDOR_LENGTH && x1<=VIDOR_WEIGHT && y1<=VIDOR_LENGTH && alpha<=1) {
    rpc[0] = MB_DEV_GFX | GFX_LN;
    rpc[1] = x0;
    rpc[2] = y0;
    rpc[3] = x1;
    rpc[4] = y1;
    rpc[5] = (color & 0x7FFF)|(alpha<<15);
    VidorMailbox.sendCommand(rpc, 6);
  }
}
void Vidor_GFX::drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, uint16_t alpha) {
  uint32_t rpc[7];
  if(x<=VIDOR_WEIGHT && y<=VIDOR_LENGTH && x+w<=VIDOR_WEIGHT && y+h<=VIDOR_LENGTH && alpha<=1) {
    rpc[0] = MB_DEV_GFX | GFX_DR;
    rpc[1] = x;
    rpc[2] = y;
    rpc[3] = w;
    rpc[4] = h;
    rpc[5] = (color&0x7FFF)|(alpha<<15);
    VidorMailbox.sendCommand(rpc, 6);
  }
}

void Vidor_GFX::fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, uint16_t alpha) {
  uint32_t rpc[7];
  if(x<=VIDOR_WEIGHT && y<=VIDOR_LENGTH && x+w<=VIDOR_WEIGHT && y+h<=VIDOR_LENGTH && alpha<=1) {
    rpc[0] = MB_DEV_GFX | GFX_FR;
    rpc[1] = x;
    rpc[2] = y;
    rpc[3] = w;
    rpc[4] = h;
    rpc[5] = (color&0x7FFF)|(alpha<<15);
    VidorMailbox.sendCommand(rpc, 6);
  }
}

void Vidor_GFX::drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color, uint16_t alpha) {
  uint32_t rpc[6];
  if(x0<=VIDOR_WEIGHT && y0<=VIDOR_LENGTH && x0+r<=VIDOR_WEIGHT
                                        && x0-r>0 && y0-r>0 && alpha<=1) {
    rpc[0] = MB_DEV_GFX | GFX_DC;
    rpc[1] = x0;
    rpc[2] = y0;
    rpc[3] = r;
    rpc[4] = (color&0x7FFF)|(alpha<<15);
    VidorMailbox.sendCommand(rpc, 5);
  }
}

void Vidor_GFX::fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color, uint16_t alpha) {
  uint32_t rpc[6];
  if(x0<=VIDOR_WEIGHT && y0<=VIDOR_LENGTH && x0+r<=VIDOR_WEIGHT
    && x0-r>0 && y0-r>0 && alpha<=1) {
    rpc[0] = MB_DEV_GFX | GFX_FC;
    rpc[1] = x0;
    rpc[2] = y0;
    rpc[3] = r;
    rpc[4] = (color&0x7FFF)|(alpha<<15);
    VidorMailbox.sendCommand(rpc, 5);
  }
}

void Vidor_GFXtext::setColor(uint16_t color) {
  txt_color=color;
}

void Vidor_GFXtext::setAlpha(uint16_t alpha) {
  txt_alpha=alpha;
}

void Vidor_GFXtext::setCursor(uint16_t x,uint16_t y) {
  cursor_x=x;
  cursor_y=y;
}

uint16_t Vidor_GFXtext::getCursorX(void) {
  return cursor_x;
}

uint16_t Vidor_GFXtext::getCursorY(void) {
  return cursor_y;
}

void Vidor_GFXtext::setSize(uint16_t size) {
  txt_size=size;
}

size_t Vidor_GFX::write(uint8_t c) {
  uint32_t rpc[6];
  size_t ret;
  if(text.cursor_x<VIDOR_WEIGHT && text.cursor_y <VIDOR_LENGTH) {
    if(c=='\n') {
      text.cursor_y+=YSHIFT*text.txt_size;
      return 1;
    } else if (c=='\r') {
      text.cursor_x=0;
      return 1;
    } else {
      rpc[0] = MB_DEV_GFX | GFX_WC;
      rpc[1] = text.cursor_x;
      rpc[2] = text.cursor_y;
      rpc[3] = ( text.txt_color&0x7FFF)|(text.txt_alpha<<15);
      rpc[4] = text.txt_size;
      rpc[5] = c;
      ret= VidorMailbox.sendCommand(rpc, 6);
      text.cursor_x+=YSHIFT*text.txt_size;
      if((text.cursor_x+(YSHIFT*text.txt_size))>=VIDOR_WEIGHT) {
        text.cursor_y+=(YSHIFT*text.txt_size);
        if(text.cursor_y>=VIDOR_LENGTH) {
          return 0;
        }
        text.cursor_x=0;
      }
      return ret;
    }
  }
}

// Downgrade 24-bit color to 16-bit (add reverse gamma lookup here?)
uint16_t Vidor_GFX::Color(uint8_t r, uint8_t g, uint8_t b) {
  return (((uint16_t)(r & 0xF8) << 8) |
         ((uint16_t)(g & 0xFC) << 3) |
                    (b         >> 3))&0x7FFF;
}

uint16_t Vidor_GFX::Red() {
  return Color(0xFF,0,0);
}

uint16_t Vidor_GFX::Green() {
  return Color(0,0xFF,0);
}

uint16_t Vidor_GFX::Blue() {
  return Color(0,0,0xFF);
}

uint16_t Vidor_GFX::Yellow() {
  return Color(0xFF,0xFF,0);
}

uint16_t Vidor_GFX::lightBlue() {
  return Color(0,0xFF,0xFF);
}

uint16_t Vidor_GFX::Purple() {
  return Color(0xFF,0,0xFF);
}

uint16_t Vidor_GFX::Black() {
  return Color(0,0,0);
}

uint16_t Vidor_GFX::Brown() {
 return Color(80,0,0);
}

uint16_t Vidor_GFX::White() {
 return Color(0xFF,0xFF,0xFF);
}

/**
 *
 */
void Vidor_GFX::Cross(uint16_t x, uint16_t y, uint16_t c, uint16_t alpha) {
  if (x<10) x=10;
  if (y<10) y=10;
  drawLine(x-10, y, x+10, y, c,alpha);
  drawLine(x, y-10, x, y+10, c,alpha);
}
