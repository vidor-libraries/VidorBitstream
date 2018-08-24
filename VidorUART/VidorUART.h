/*
  Copyright (c) 2015 Arduino LLC.  All right reserved.
  Copyright (c) 2018 Arduino SA. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#pragma once

#include "VidorFPGA.h"
#include "defines.h"
#include "HardwareSerial.h"
#include "RingBuffer.h"

#include <cstddef>

class VidorUart : public HardwareSerial
{
  public:
    VidorUart(int _idx,int _tx,int _rx,int _cts,int _rts,int _dtr,int _dsr);
    void begin(unsigned long baudRate);
    void begin(unsigned long baudrate, uint16_t config);
    void end();
    void enableUART(int tx, int rx);
    void setUART(int baud, int config);
    void disableUART();

    int available();
    int availableForWrite();
    int peek();
    int read();
    int read(uint8_t* data, size_t len);
    void flush();
    size_t write(const uint8_t data);
    int enableFlowControl(void);
    using Print::write; // pull in write(str) and write(buf, size) from Print
    size_t write(const uint8_t* data, size_t len);

    void onInterrupt();

    operator bool() {
      return true;
    }

  private:
    int idx;
    int tx;
    int rx;
    int cts;
    int rts;
    int dtr;
    int dsr;
    int index;
    RingBuffer rxBuffer;
    RingBuffer txBuffer;
    uint8_t devIdx;
};

extern VidorUart SerialEx;
extern VidorUart SerialFPGA0;
extern VidorUart SerialFPGA1;
extern VidorUart SerialFPGA2;
extern VidorUart SerialFPGA3;
extern VidorUart SerialFPGA4;
extern VidorUart SerialFPGA5;
extern VidorUart SerialFPGA6;
extern VidorUart SerialFPGA7;
