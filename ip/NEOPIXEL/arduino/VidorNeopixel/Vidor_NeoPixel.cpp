/*
  This file is part of the VidorPeripherals/VidorGraphics library.
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

#include "VidorFPGA.h"
#include "VidorMailbox.h"

#include "Vidor_NeoPixel.h"

Vidor_NeoPixel::Vidor_NeoPixel(uint16_t howMany, uint16_t pin, uint8_t type)
{

  this->howMany = howMany;
  this->bufLen = howMany;
  this->type = type;
  this->pin = pin;
}

int Vidor_NeoPixel::begin()
{
  uint32_t rpc[7];

  if (initialized == true) {
    return 0;
  }

  int ret = init(NEOPIXEL_UID, digital_to_fpga(pin));
  if (ret < 0) {
    return -1;
  }

  rpc[0] = RPC_CMD(info.giid, info.chn, 2);
  rpc[1] = howMany;
  rpc[2] = type;
  rpc[3] = bufLen;
  rpc[4] = zzf;
  rpc[5] = zzl;
  rpc[6] = flags;

  initialized = true;

  return VidorMailbox.sendCommand(rpc, 7);
}

int Vidor_NeoPixel::end()
{
  if (!initialized) {
    return 0;
  }
  initialized = false;
  uint32_t rpc[1];
  rpc[0] = RPC_CMD(info.giid, info.chn, 4);
  return VidorMailbox.sendCommand(rpc, 1);
}

uint32_t Vidor_NeoPixel::setZZ(bool enabled, uint32_t len) {
  this->zzf = enabled ? 1 : 0;
  this->zzl = len;
  return 0;
}

uint32_t Vidor_NeoPixel::setFlags(uint32_t flags) {
  this->flags = flags;
  return 0;
}

uint32_t Vidor_NeoPixel::setPin(uint8_t pin) {
  this->pin = pin;
  begin();
  return 0;
}

/**
 *
 */
uint32_t Vidor_NeoPixel::setPixelColor(uint16_t n, uint32_t red, uint32_t green, uint32_t blue, uint32_t white)
{

  if (!initialized) {
    begin();
  }

  uint32_t rpc[6];

  rpc[0] = RPC_CMD(info.giid, info.chn, 6);
  rpc[1] = n;
  rpc[2] = red;
  rpc[3] = green;
  rpc[4] = blue;
  rpc[5] = white;

  return VidorMailbox.sendCommand(rpc, 6);
}

/**
 *
 */
uint32_t Vidor_NeoPixel::setBrightness(uint16_t brg)
{

  if (!initialized) {
    begin();
  }

  uint32_t rpc[2];

  rpc[0] = RPC_CMD(info.giid, info.chn, 7);
  rpc[1] = brg;

  return VidorMailbox.sendCommand(rpc, 2);
}

/**
 *
 */
uint32_t Vidor_NeoPixel::show(void)
{

  if (!initialized) {
    begin();
  }

  uint32_t rpc[2];

  rpc[0] = RPC_CMD(info.giid, info.chn, 8);
  rpc[1] = offset;

  return VidorMailbox.sendCommand(rpc, 2);
}


/**
 *
 */
uint32_t Vidor_NeoPixel::setTimings(uint32_t freq, uint32_t trst, uint32_t t0h, uint32_t t1h, uint32_t ttot)
{

  if (!initialized) {
    begin();
  }

  uint32_t rpc[6];

  rpc[0] = RPC_CMD(info.giid, info.chn, 5);
  rpc[1] = freq;
  rpc[2] = trst;
  rpc[3] = t0h;
  rpc[4] = t1h;
  rpc[5] = ttot;

  return VidorMailbox.sendCommand(rpc, 6);
}

/**
 *
 */
uint32_t Vidor_NeoPixel::setWrap(uint32_t start, uint32_t len, uint32_t restart)
{

  if (!initialized) {
    begin();
  }

  uint32_t rpc[4];

  rpc[0] = RPC_CMD(info.giid, info.chn, 9);
  rpc[1] = start;
  rpc[2] = len;
  rpc[3] = restart;

  return VidorMailbox.sendCommand(rpc, 4);
}