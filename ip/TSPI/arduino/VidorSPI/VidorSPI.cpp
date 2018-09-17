/*
 * SPI Master library for Arduino Zero.
 * Copyright (c) 2015 Arduino LLC
 * Copyright (c) 2018 Arduino SA. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "VidorSPI.h"
#include "VidorIO.h"
#include <Arduino.h>
#include <wiring_private.h>
#include <assert.h>

#define SPI_IMODE_NONE   0
#define SPI_IMODE_EXTINT 1
#define SPI_IMODE_GLOBAL 2

#define MAX_CLOCK_FREQUENCY   50000000  //50MHz

VidorSPIClass::VidorSPIClass(int index,int _mosi,int _miso,int _sck,int _cs) : settings(SPISettings(0, MSBFIRST, SPI_MODE0))
{
  initialized = false;
  this->index = index;
  mosi = _mosi;
  miso = _miso;
  sck  = _sck;
  cs   = _cs;
  // TODO nuova implementazione FPGA devIdx = FPGA.devIdxGet(FPGA_SPI_DID);
  devIdx = MB_DEV_SPI;
}

void VidorSPIClass::begin()
{
  init();
  config(DEFAULT_SPI_SETTINGS);
}

void VidorSPIClass::init()
{
  if (initialized)
    return;
  interruptMode = SPI_IMODE_NONE;
  interruptSave = 0;
  interruptMask = 0;
  initialized = true;

  //VidorIO::enableSPI(index, mosi, miso, sck, cs);
  if (mosi == -1) {
    VidorIO::pinMode(FPGA_NINA_MOSI, 4);
  } else if (mosi <= 14) {
    mosi = mosi - 0;
    pinModeExtended(mosi + 140, 5);
  } else {
    mosi = mosi - A0;
    pinModeExtended(mosi + 133, 5);
  }
  if (miso == -1) {
    VidorIO::pinMode(FPGA_NINA_MISO, INPUT);
  } else if (miso <= 14) {
    miso = miso - 0 ;
    pinModeExtended(miso + 140, INPUT);
  } else {
    miso = miso - A0;
    pinModeExtended(miso + 133, INPUT);
  }
  if (sck == -1) {
    VidorIO::pinMode(FPGA_NINA_SCK, 4);
  } else if (sck <= 14) {
    sck = sck - 0 ;
    pinModeExtended(sck + 140, 5);
  } else {
    sck = sck - A0;
    pinModeExtended(sck + 133, 5);
  }

  if (cs == -1) {
    goto enable_spi;
  } else if (cs <= 14) {
    cs = cs - 0;
    pinModeExtended(cs + 140, 5);
  } else {
    cs = cs - A0;
    pinModeExtended(cs + 133, 5);
  }

enable_spi:
  uint32_t rpc[2];
  rpc[0] = MB_CMD(devIdx, index, 0, 0x01);
  rpc[1] = index;
  VidorMailbox.sendCommand(rpc, 2);
}

void VidorSPIClass::setSPIMode(int index, int baud, int mode, int bitOrder)
{
  uint32_t rpc[4];
  rpc[0] = MB_CMD(devIdx, index, 0, 0x02);
  rpc[1] = baud * 2;
  rpc[2] = 0;
  rpc[3] = 0;
  VidorMailbox.sendCommand(rpc, 4);
}

void VidorSPIClass::config(SPISettings settings)
{
  if (this->settings != settings) {
    this->settings.clockFreq = settings.getClockFreq();
    this->settings.dataMode = settings.getDataMode();
    this->settings.bitOrder = settings.getBitOrder();
    setSPIMode(index, this->settings.clockFreq, this->settings.dataMode, this->settings.bitOrder);
  }
}

void VidorSPIClass::end()
{
  uint32_t rpc[1];

  rpc[0] = MB_CMD(devIdx, index, 0, 0x03);
  VidorMailbox.sendCommand(rpc, 1);
  initialized = false;
}

#ifndef interruptsStatus
#define interruptsStatus() __interruptsStatus()
static inline unsigned char __interruptsStatus(void) __attribute__((always_inline, unused));
static inline unsigned char __interruptsStatus(void)
{
  // See http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0497a/CHDBIBGJ.html
  return (__get_PRIMASK() ? 0 : 1);
}
#endif

void VidorSPIClass::usingInterrupt(int interruptNumber)
{
  if ((interruptNumber == NOT_AN_INTERRUPT) || (interruptNumber == EXTERNAL_INT_NMI))
    return;

  uint8_t irestore = interruptsStatus();
  noInterrupts();

  if (interruptNumber >= EXTERNAL_NUM_INTERRUPTS)
    interruptMode = SPI_IMODE_GLOBAL;
  else
  {
    interruptMode |= SPI_IMODE_EXTINT;
    interruptMask |= (1 << interruptNumber);
  }

  if (irestore)
    interrupts();
}

void VidorSPIClass::notUsingInterrupt(int interruptNumber)
{
  if ((interruptNumber == NOT_AN_INTERRUPT) || (interruptNumber == EXTERNAL_INT_NMI))
    return;

  if (interruptMode & SPI_IMODE_GLOBAL)
    return; // can't go back, as there is no reference count

  uint8_t irestore = interruptsStatus();
  noInterrupts();

  interruptMask &= ~(1 << interruptNumber);

  if (interruptMask == 0)
    interruptMode = SPI_IMODE_NONE;

  if (irestore)
    interrupts();
}

void VidorSPIClass::beginTransaction(SPISettings settings)
{
  if (interruptMode != SPI_IMODE_NONE)
  {
    if (interruptMode & SPI_IMODE_GLOBAL)
    {
      interruptSave = interruptsStatus();
      noInterrupts();
    }
    else if (interruptMode & SPI_IMODE_EXTINT)
      EIC->INTENCLR.reg = EIC_INTENCLR_EXTINT(interruptMask);
  }

  
  config(settings);
}

void VidorSPIClass::endTransaction(void)
{
  if (interruptMode != SPI_IMODE_NONE)
  {
    if (interruptMode & SPI_IMODE_GLOBAL)
    {
      if (interruptSave)
        interrupts();
    }
    else if (interruptMode & SPI_IMODE_EXTINT)
      EIC->INTENSET.reg = EIC_INTENSET_EXTINT(interruptMask);
  }
}

void VidorSPIClass::setBitOrder(BitOrder order)
{
  settings.bitOrder = order;
  setSPIMode(index, settings.clockFreq, settings.dataMode, settings.bitOrder);
}

void VidorSPIClass::setDataMode(uint8_t mode)
{
  settings.dataMode = mode;
  setSPIMode(index, settings.clockFreq, settings.dataMode, settings.bitOrder);
}

void VidorSPIClass::setClockDivider(uint8_t div)
{
  settings.clockFreq = MAX_CLOCK_FREQUENCY / div;
  setSPIMode(index, settings.clockFreq, settings.dataMode, settings.bitOrder);
}

byte VidorSPIClass::transfer(uint8_t data)
{
  transfer(&data, 1);
  return data;
}

uint16_t VidorSPIClass::transfer16(uint16_t data) {
  union { uint16_t val; struct { uint8_t lsb; uint8_t msb; }; } t;

  t.val = data;

  if (settings.bitOrder == LSBFIRST) {
    t.lsb = transfer(t.lsb);
    t.msb = transfer(t.msb);
  } else {
    t.msb = transfer(t.msb);
    t.lsb = transfer(t.lsb);
  }

  return t.val;
}

void VidorSPIClass::transfer(void *buf, size_t count)
{
  //VidorIO::transferDataSPI(index, (uint8_t*)buf, count);
  uint32_t rpc[256];
  if (count > 128) {
    return;
  }
  rpc[0] = MB_CMD(devIdx, index, 0, 0x04);
  rpc[1] = count;
  memcpy(&rpc[2], buf, count);
  int ret = VidorMailbox.sendCommand(rpc, 2+(rpc[1]+3)/4);

  VidorMailbox.read(2, &rpc[2], (count+3)/4);
  memcpy(buf, &rpc[2], count);
}

void VidorSPIClass::attachInterrupt() {
  // Should be enableInterrupt()
}

void VidorSPIClass::detachInterrupt() {
  // Should be disableInterrupt()
}

VidorSPIClass SPIFPGA0 (1, A2, A6, A3, A4);
VidorSPIClass SPIFPGA1 (2, A5, 0, 1, 2);
VidorSPIClass SPIFPGA2 (3, 3, 7, 4, 5);
VidorSPIClass SPIFPGA3 (4, 6, 8, 9, 10);
VidorSPIClass SPIFPGA4 (5, 11, 12, 13, 14);

VidorSPIClass SPIEx (0, -1, -1, -1, -1);  // Internally connected to NiNa module
