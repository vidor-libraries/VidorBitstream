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

#ifndef VidorTwoWire_h
#define VidorTwoWire_h

#include "Stream.h"
#include "RingBuffer.h"
#include "defines.h"

 // WIRE_HAS_END means Wire has end()
#define WIRE_HAS_END 1

#define READ_FLAG		1
#define WRITE_FLAG		0

class VidorTwoWire : public Stream
{
  public:
    VidorTwoWire(int index,int _scl,int _sda);
    int begin();
    void begin(uint8_t address);
    void end();
    void setClock(uint32_t);

    void beginTransmission(uint8_t);
    uint8_t endTransmission(bool stopBit);
    uint8_t endTransmission(void);

    uint8_t requestFrom(uint8_t address, size_t quantity, bool stopBit);
    uint8_t requestFrom(uint8_t address, size_t quantity);

    size_t write(uint8_t data);
    size_t write(const uint8_t * data, size_t quantity);

    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual void flush(void);
    void onReceive(void(*)(int));
    void onRequest(void(*)(void));

    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    using Print::write;

    void onService(void);

  private:
    bool transmissionBegun;
    uint8_t idx;
    int scl;
    int sda;
    // RX Buffer
    RingBufferN<256> rxBuffer;

    //TX buffer
    RingBufferN<256> txBuffer;
    uint8_t txAddress;

    // Callback user functions
    void (*onRequestCallback)(void);
    void (*onReceiveCallback)(int);

    // TWI clock frequency
    static const uint32_t TWI_CLOCK = 100000;
    uint8_t devIdx;
};

extern VidorTwoWire WireFPGA0;
extern VidorTwoWire WireFPGA1;
extern VidorTwoWire WireFPGA2;
extern VidorTwoWire WireFPGA3;
extern VidorTwoWire WireEx;

#endif
