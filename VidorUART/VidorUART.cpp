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

#include "VidorUART.h"
#include "Arduino.h"

VidorUart::VidorUart(int _idx,int _tx,int _rx,int _cts,int _rts,int _dtr,int _dsr)
{
  index = idx;
  idx=_idx;
  tx=_tx;
  rx=_rx;
  cts=_cts;
  rts=_rts;
  dtr=_dtr;
  dsr=_dsr;
}

void VidorUart::begin(unsigned long baudrate)
{
  begin(baudrate, SERIAL_8N1);
}

void VidorUart::begin(unsigned long baudrate, uint16_t config)
{
  VidorIO::enableUART(tx,rx);
  VidorIO::setUART(index, baudrate, config);
}

void VidorUart::end()
{
  VidorIO::disableUART(index);
  rxBuffer.clear();
  txBuffer.clear();
}

void VidorUart::flush()
{
  while(txBuffer.available()); // wait until TX buffer is empty
  VidorIO::flushUART(index);
}

int VidorUart::available()
{
  int ret = VidorIO::availableUART(index);
  if (ret > 0) {
    // Workaround until we have interrupt capabilities
    onInterrupt();
  }
  return ret;
}

void VidorUart::onInterrupt()
{
  int available = VidorIO::availableUART(index);
  while (available-- > 0) {
    rxBuffer.store_char(VidorIO::readUART(index));
  }
}

int VidorUart::availableForWrite()
{
  return txBuffer.availableForStore();
}

int VidorUart::peek()
{
  return rxBuffer.peek();
}

int VidorUart::read()
{
  return VidorIO::readUART(index);
}

size_t VidorUart::write(const uint8_t data)
{
  VidorIO::writeUART(index, data);
  return 1;
}


size_t VidorUart::write(const uint8_t* data, size_t len)
{
  VidorIO::writeUART(index, (uint8_t*)data, len);
  return len;
}

int VidorUart::enableFlowControl(void)
{
  if(rts>=0 && cts>=0 && dtr>=0 && dsr>=0 ) {
    VidorIO::enableUART(rts,cts);
    VidorIO::enableUART(dtr,dsr);
    return 1;
  }
  return 0;
}


#if FPGA_UART_INTERFACES_COUNT > 0
VidorUart SerialFPGA0(0,  A0, A1, -1, -1, -1, -1);
#if FPGA_UART_INTERFACES_COUNT > 1
VidorUart SerialFPGA1(1,  A2, A3, A0, A1, -1, -1);
#if FPGA_UART_INTERFACES_COUNT > 2
VidorUart SerialFPGA2(2,  A4, A5, -1, -1, -1, -1);
#if FPGA_UART_INTERFACES_COUNT > 3
VidorUart SerialFPGA3(3,  A6,  0, A4, A5, A3, A2);
#if FPGA_UART_INTERFACES_COUNT > 4
VidorUart SerialFPGA4(4,   1,  2, -1, -1, -1, -1);
#if FPGA_UART_INTERFACES_COUNT > 5
VidorUart SerialFPGA5(5,   3,  4,  1,  2,  0, A6);
#if FPGA_UART_INTERFACES_COUNT > 6
VidorUart SerialFPGA6(6,   5,  6, -1, -1, -1, -1);
#if FPGA_UART_INTERFACES_COUNT > 7
VidorUart SerialFPGA7(7,   7,  8,  5,  6,  4,  3);
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif