/*
 * SPI Master library for Arduino Zero.
 * Copyright (c) 2015 Arduino LLC
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
  sck = _sck;
  cs = _cs;
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
  VidorIO::enableSPI(index, mosi, miso, sck, cs);
}

void VidorSPIClass::config(SPISettings settings)
{
  if (this->settings != settings) {
    this->settings.clockFreq = settings.getClockFreq();
    this->settings.dataMode = settings.getDataMode();
    this->settings.bitOrder = settings.getBitOrder();
    VidorIO::setSPIMode(index, this->settings.clockFreq, this->settings.dataMode, this->settings.bitOrder);
  }
}

void VidorSPIClass::end()
{
  VidorIO::disableSPI(index);
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
  VidorIO::setSPIMode(index, settings.clockFreq, settings.dataMode, settings.bitOrder);
}

void VidorSPIClass::setDataMode(uint8_t mode)
{
  settings.dataMode = mode;
  VidorIO::setSPIMode(index, settings.clockFreq, settings.dataMode, settings.bitOrder);
}

void VidorSPIClass::setClockDivider(uint8_t div)
{
  settings.clockFreq = MAX_CLOCK_FREQUENCY / div;
  VidorIO::setSPIMode(index, settings.clockFreq, settings.dataMode, settings.bitOrder);
}

byte VidorSPIClass::transfer(uint8_t data)
{
  return VidorIO::transferDataSPI(index, data);
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
  VidorIO::transferDataSPI(index, (uint8_t*)buf, count);
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
