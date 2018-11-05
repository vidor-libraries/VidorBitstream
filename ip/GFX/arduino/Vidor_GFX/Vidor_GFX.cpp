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

Vidor_GFX::Vidor_GFX() {
}

Vidor_GFX::Vidor_GFX(Vidor_NeoPixel& instance) {
  framebuffer = &instance;
}

void Vidor_GFXbuffer::scroll(int delay, ScrollDirection direction) {
  privateScroll(NP_SEQ_FLG_START | NP_SEQ_FLG_BUF_LOOP | ((direction == LEFT_TO_RIGHT) ? NP_SEQ_FLG_INV_LOOP : 0), delay);
}

void Vidor_GFXbuffer::noScroll() {
  privateScroll(NP_SEQ_FLG_STOP, 0);
}

void Vidor_GFXbuffer::privateScroll(int flags, int delay) {
  if (!np->initialized) {
    begin();
  }

  uint32_t rpc[4];

  rpc[0] = RPC_CMD(np->info.giid, np->info.chn, 11);
  rpc[1] = flags;
  rpc[2] = 0;     // unused ???
  rpc[3] = delay;

  VidorMailbox.sendCommand(rpc, 4);
}

void Vidor_GFXbuffer::begin(bool rotate90, bool flipV, bool flipH) {
  uint32_t rpc[6];

  // TODO: ???? how should I integrate this?
  rpc[0] = RPC_CMD(np->info.giid, np->info.chn, 2);
  rpc[1] = x*y;
  rpc[2] = zigzag?1:0;
  rpc[3] = y;
  rpc[4] = (rotate90 ? GFX_GC_ROT90 : 0) | (flipV ? GFX_GC_FLIP_V : 0) | (flipH ? GFX_GC_FLIP_H : 0);

  VidorMailbox.sendCommand(rpc, 6);
  setup = true;

  // late init of neopixel core
  np->initialized = false;
  np->begin();
};


/* Vidor_GFX */

int Vidor_GFX::begin() {
  int ret = init(GFX_UID);
  if (ret < 0) {
    return ret;
  }
  uint32_t rpc[1];
  rpc[0] = RPC_CMD(info.giid, info.chn, 2);
  return VidorMailbox.sendCommand(rpc, 1);
}

void Vidor_GFX::drawPixel(uint16_t x, uint16_t y, uint32_t color, uint8_t alpha) {
  uint32_t rpc[5];
  if(x<=VIDOR_WIDTH && y<=VIDOR_HEIGHT) {
    rpc[0] = RPC_CMD(info.giid, info.chn, 5);
    rpc[1] = x;
    rpc[2] = y;
    rpc[3] = (color & 0xFFFFFF) | ((uint32_t)alpha << 24);
    VidorMailbox.sendCommand(rpc, 4);
  }
}

void Vidor_GFX::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color, uint8_t alpha) {
  uint32_t rpc[7];
  if(x0<=VIDOR_WIDTH && y0<=VIDOR_HEIGHT && x1<=VIDOR_WIDTH && y1<=VIDOR_HEIGHT ) {
    rpc[0] = RPC_CMD(info.giid, info.chn, 6);
    rpc[1] = x0;
    rpc[2] = y0;
    rpc[3] = x1;
    rpc[4] = y1;
    rpc[5] = (color & 0xFFFFFF) | ((uint32_t)alpha << 24);
    VidorMailbox.sendCommand(rpc, 6);
  }
}
void Vidor_GFX::drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color, uint8_t alpha) {
  uint32_t rpc[7];
  if(x<=VIDOR_WIDTH && y<=VIDOR_HEIGHT && x+w<=VIDOR_WIDTH && y+h<=VIDOR_HEIGHT) {
    rpc[0] = RPC_CMD(info.giid, info.chn, 7);
    rpc[1] = x;
    rpc[2] = y;
    rpc[3] = w;
    rpc[4] = h;
    rpc[5] = (color & 0xFFFFFF) | ((uint32_t)alpha << 24);
    VidorMailbox.sendCommand(rpc, 6);
  }
}

void Vidor_GFX::fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color, uint8_t alpha) {
  uint32_t rpc[7];
  if(x<=VIDOR_WIDTH && y<=VIDOR_HEIGHT && x+w<=VIDOR_WIDTH && y+h<=VIDOR_HEIGHT) {
    rpc[0] = RPC_CMD(info.giid, info.chn, 8);
    rpc[1] = x;
    rpc[2] = y;
    rpc[3] = w;
    rpc[4] = h;
    rpc[5] = (color & 0xFFFFFF) | ((uint32_t)alpha << 24);
    VidorMailbox.sendCommand(rpc, 6);
  }
}

void Vidor_GFX::drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint32_t color, uint8_t alpha) {
  uint32_t rpc[6];
  if(x0<=VIDOR_WIDTH && y0<=VIDOR_HEIGHT && x0+r<=VIDOR_WIDTH
                                        && x0-r>0 && y0-r>0) {
    rpc[0] = RPC_CMD(info.giid, info.chn, 9);
    rpc[1] = x0;
    rpc[2] = y0;
    rpc[3] = r;
    rpc[4] = (color & 0xFFFFFF) | ((uint32_t)alpha << 24);
    VidorMailbox.sendCommand(rpc, 5);
  }
}

void Vidor_GFX::fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint32_t color, uint8_t alpha) {
  uint32_t rpc[6];
  if(x0<=VIDOR_WIDTH && y0<=VIDOR_HEIGHT && x0+r<=VIDOR_WIDTH
    && x0-r>0 && y0-r>0) {
    rpc[0] = RPC_CMD(info.giid, info.chn, 10);
    rpc[1] = x0;
    rpc[2] = y0;
    rpc[3] = r;
    rpc[4] = (color & 0xFFFFFF) | ((uint32_t)alpha << 24);
    VidorMailbox.sendCommand(rpc, 5);
  }
}

void Vidor_GFXtext::setFont(uint32_t num) {
  uint32_t rpc[2];
  rpc[0] = RPC_CMD(parent->info.giid, parent->info.chn, 13);
  rpc[1] = num;
  VidorMailbox.sendCommand(rpc, 2);
}

void Vidor_GFXtext::setColor(uint32_t color) {
  this->color = color;
}

void Vidor_GFXtext::setCursor(uint16_t x,uint16_t y) {
  this->x = x;
  this->y = y;
}

void Vidor_GFXtext::setSize(uint16_t size) {
  this->size = size;
}

size_t Vidor_GFXtext::write(uint8_t c) {
  uint32_t rpc[6];
  rpc[0] = RPC_CMD(parent->info.giid, parent->info.chn, 11);
  rpc[1] = x;
  rpc[2] = y;
  rpc[3] = color;
  rpc[4] = size;
  rpc[5] = c;
  VidorMailbox.sendCommand(rpc, 6);
  setCursor(x+(size*7), c == '\n' ? y+(size*7) : y);
  return 1;
}

size_t Vidor_GFXtext::write(char* string, int len) {
  uint32_t rpc[256];
  rpc[0] = RPC_CMD(parent->info.giid, parent->info.chn, 12);
  rpc[1] = x;
  rpc[2] = y;
  rpc[3] = color;
  memcpy((char*)&rpc[4], string, len);
  VidorMailbox.sendCommand(rpc, 4 + ((len+3)/4));
  return len;
}

size_t Vidor_GFX::write(uint8_t c) {
  return text.write(c);
}

uint32_t Vidor_GFX::Color(uint8_t r, uint8_t g, uint8_t b) {
  return (0xFF << 24) | ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}

uint32_t Vidor_GFX::Red() {
  return Color(0xFF, 0, 0);
}

uint32_t Vidor_GFX::Green() {
  return Color(0, 0xFF, 0);
}

uint32_t Vidor_GFX::Blue() {
  return Color(0, 0, 0xFF);
}

uint32_t Vidor_GFX::Yellow() {
  return Color(0xFF, 0xFF, 0);
}

uint32_t Vidor_GFX::lightBlue() {
  return Color(0, 0xFF, 0xFF);
}

uint32_t Vidor_GFX::Purple() {
  return Color(0xFF, 0, 0xFF);
}

uint32_t Vidor_GFX::Black() {
  return Color(0, 0, 0);
}

uint32_t Vidor_GFX::Brown() {
 return Color(80, 0, 0);
}

uint32_t Vidor_GFX::White() {
 return Color(0xFF, 0xFF, 0xFF);
}

/**
 *
 */
void Vidor_GFX::Cross(uint16_t x, uint16_t y, uint32_t c, uint8_t alpha) {
  if (x<10) x=10;
  if (y<10) y=10;
  drawLine(x-10, y, x+10, y, c, alpha);
  drawLine(x, y-10, x, y+10, c, alpha);
}
