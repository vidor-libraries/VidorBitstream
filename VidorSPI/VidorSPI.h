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

#ifndef _VIDORSPI_H_INCLUDED
#define _VIDORSPI_H_INCLUDED

#include <Arduino.h>
#include <SPI.h>
#include "defines.h"
#include "VidorIO.h"

class VidorSPISettings {
  public:
  VidorSPISettings(uint32_t clock, BitOrder bitOrder, uint8_t dataMode) {
    if (__builtin_constant_p(clock)) {
      init_AlwaysInline(clock, bitOrder, dataMode);
    } else {
      init_MightInline(clock, bitOrder, dataMode);
    }
  }

  // Default speed set to 4MHz, SPI mode set to MODE 0 and Bit order set to MSB first.
  VidorSPISettings() { init_AlwaysInline(4000000, MSBFIRST, SPI_MODE0); }

  VidorSPISettings(SPISettings& settings) {
	  init_MightInline(settings.getClockFreq(), settings.getBitOrder(), settings.getDataMode());
  }

  VidorSPISettings(SPISettings settings) {
	  init_MightInline(settings.getClockFreq(), settings.getBitOrder(), settings.getDataMode());
  }

  bool operator==(const VidorSPISettings& rhs) const
  {
    if ((this->clockFreq == rhs.clockFreq) &&
        (this->bitOrder == rhs.bitOrder) &&
        (this->dataMode == rhs.dataMode)) {
      return true;
    }
    return false;
  }

  bool operator!=(const VidorSPISettings& rhs) const
  {
    return !(*this == rhs);
  }

  bool operator==(const SPISettings& rhs) const
  {
    if ((this->clockFreq == rhs.getClockFreq()) &&
        (this->bitOrder == rhs.getBitOrder()) &&
        (this->dataMode == rhs.getDataMode())) {
      return true;
    }
    return false;
  }

  bool operator!=(const SPISettings& rhs) const
  {
    return !(*this == rhs);
  }

  private:
  void init_MightInline(uint32_t clock, BitOrder bitOrder, uint8_t dataMode) {
    init_AlwaysInline(clock, bitOrder, dataMode);
  }

  void init_AlwaysInline(uint32_t clock, BitOrder bitOrder, uint8_t dataMode) __attribute__((__always_inline__)) {
    this->clockFreq = clock;
    this->bitOrder = bitOrder;
    this->dataMode = dataMode;
  }

  uint32_t clockFreq;
  uint8_t dataMode;
  BitOrder bitOrder;

  friend class VidorSPIClass;
  friend class BitBangedSPI;
};


class VidorSPIClass {
  public:
  VidorSPIClass(int index,int _mosi,int _miso,int _sck,int _cs);

  byte transfer(uint8_t data);
  uint16_t transfer16(uint16_t data);
  void transfer(void *buf, size_t count);

  void setSPIMode(int index, int baud, int mode, int bitOrder);

  // Transaction Functions
  void usingInterrupt(int interruptNumber);
  void notUsingInterrupt(int interruptNumber);
  void beginTransaction(SPISettings settings);
  void endTransaction(void);

  // SPI Configuration methods
  void attachInterrupt();
  void detachInterrupt();

  void begin();
  void end();

  void setBitOrder(BitOrder order);
  void setDataMode(uint8_t uc_mode);
  void setClockDivider(uint8_t uc_div);

  private:
  void init();
  void config(SPISettings settings);

  VidorSPISettings settings;
  int mosi;
  int miso;
  int sck;
  int cs;
  int index;
  bool initialized;
  uint8_t interruptMode;
  char interruptSave;
  uint32_t interruptMask;
  uint8_t devIdx;
};

extern VidorSPIClass SPIFPGA0;
extern VidorSPIClass SPIFPGA1;
extern VidorSPIClass SPIFPGA2;
extern VidorSPIClass SPIFPGA3;
extern VidorSPIClass SPIFPGA4;
extern VidorSPIClass SPIEx;

#endif
