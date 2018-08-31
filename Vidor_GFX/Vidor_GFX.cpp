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

Vidor_GFX::Vidor_GFX(int index) {
  text = Vidor_GFXtext(index);
  idx = index;
  // TODO nuova implementazione FPGA devIdx = FPGA.devIdxGet(FPGA_GFX_DID);
  devIdx = MB_DEV_GFX;
}

Vidor_GFX::Vidor_GFX(Vidor_NeoPixel& instance) {
  idx = instance.index + 1;
  text = Vidor_GFXtext(idx);
  // TODO nuova implementazione FPGA devIdx = FPGA.devIdxGet(FPGA_GFX_DID);
  devIdx = MB_DEV_GFX;
}

Vidor_GFXtext::Vidor_GFXtext(int index) {
  idx = index;
  // TODO nuova implementazione FPGA devIdx = FPGA.devIdxGet(FPGA_GFX_DID);
  devIdx = MB_DEV_GFX;
}

void Vidor_GFXbuffer::scroll(int delay) {
  privateScroll(NP_SEQ_FLG_START | NP_SEQ_FLG_BUF_LOOP | NP_SEQ_FLG_INV_LOOP, delay);
}

void Vidor_GFXbuffer::noScroll() {
  privateScroll(NP_SEQ_FLG_STOP, 0);
}

void Vidor_GFXbuffer::privateScroll(int flags, int delay) {
  if (!init) {
    begin();
  }

  select();
  uint32_t rpc[4];

  rpc[0] = MB_CMD(devIdx, 0, 0, 10);
  rpc[1] = flags;
  rpc[2] = 0;     // unused ???
  rpc[3] = delay;

  VidorMailbox.sendCommand(rpc, 4);
}

void Vidor_GFXbuffer::select() {
  uint32_t rpc[2];

  rpc[0] = MB_CMD(devIdx, 0, 0, 7);
  rpc[1] = idx;

  VidorMailbox.sendCommand(rpc, 2);
}

void Vidor_GFXbuffer::begin() {
  uint32_t rpc[5];

  rpc[0] = MB_CMD(devIdx, 0, 0, 6);
  rpc[1] = idx;
  rpc[2] = x*y;
  rpc[3] = zigzag?1:0;
  rpc[4] = y;

  VidorMailbox.sendCommand(rpc, 5);
  setup = true;

  // late init of neopixel core
  np->init = false;
  np->begin();
};

void Vidor_GFX::drawPixel(uint16_t x, uint16_t y, uint32_t color, uint8_t alpha) {
  uint32_t rpc[5];
  if(x<=VIDOR_WIDTH && y<=VIDOR_HEIGHT) {
    rpc[0] = MB_CMD(devIdx, idx, 0, GFX_PX);
    rpc[1] = x;
    rpc[2] = y;
    rpc[3] = (color & 0xFFFFFF) | ((uint32_t)alpha << 24);
    VidorMailbox.sendCommand(rpc, 4);
  }
}

void Vidor_GFX::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color, uint8_t alpha) {
  uint32_t rpc[7];
  if(x0<=VIDOR_WIDTH && y0<=VIDOR_HEIGHT && x1<=VIDOR_WIDTH && y1<=VIDOR_HEIGHT ) {
    rpc[0] = MB_CMD(devIdx, idx, 0, GFX_LN);
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
    rpc[0] = MB_CMD(devIdx, idx, 0, GFX_DR);
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
    rpc[0] = MB_CMD(devIdx, idx, 0, GFX_FR);
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
    rpc[0] = MB_CMD(devIdx, idx, 0, GFX_DC);
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
    rpc[0] = MB_CMD(devIdx, idx, 0, GFX_FC);
    rpc[1] = x0;
    rpc[2] = y0;
    rpc[3] = r;
    rpc[4] = (color & 0xFFFFFF) | ((uint32_t)alpha << 24);
    VidorMailbox.sendCommand(rpc, 5);
  }
}

void Vidor_GFX::setFont(uint32_t num) {
  uint32_t rpc[2];
  rpc[0] = MB_CMD(devIdx, idx, 0, GFX_SF);
  rpc[1] = num;
  VidorMailbox.sendCommand(rpc, 2);
}

void Vidor_GFXtext::setColor(uint32_t color) {
  uint32_t rpc[2];
  rpc[0] = MB_CMD(devIdx, idx, 0, GFX_SC);
  rpc[1] = color;
  VidorMailbox.sendCommand(rpc, 2);
}

void Vidor_GFXtext::setAlpha(uint8_t alpha) {
  uint32_t rpc[2];
  rpc[0] = MB_CMD(devIdx, idx, 0, GFX_SA);
  rpc[1] = alpha;
  VidorMailbox.sendCommand(rpc, 2);
}

void Vidor_GFXtext::setCursor(uint16_t x,uint16_t y) {
  uint32_t rpc[3];
  rpc[0] = MB_CMD(devIdx, idx, 0, GFX_CR);
  rpc[1] = x;
  rpc[2] = y;
  VidorMailbox.sendCommand(rpc, 3);
}

void Vidor_GFXtext::setSize(uint16_t size) {
  uint32_t rpc[2];
  rpc[0] = MB_CMD(devIdx, idx, 0, GFX_TS);
  rpc[1] = size;
  VidorMailbox.sendCommand(rpc, 2);
}

size_t Vidor_GFX::write(uint8_t c) {
  uint32_t rpc[2];
  rpc[0] = MB_CMD(devIdx, idx, 0, GFX_WC);
  rpc[1] = c;
  VidorMailbox.sendCommand(rpc, 2);
  return 1;
}

uint32_t Vidor_GFX::Color(uint8_t r, uint8_t g, uint8_t b) {
  return ((((uint32_t)r) << 16) |
          (((uint32_t)g) <<  8) |
                     (b));
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
