/*
  Copyright (c) 2015 Arduino LLC.  All right reserved.

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

VidorUart::VidorUart(VidorBase *_s, int idx)
{
  s = _s;
  index = idx;
}

void VidorUart::begin(unsigned long baudrate)
{
  begin(baudrate, SERIAL_8N1);
}

void VidorUart::begin(unsigned long baudrate, uint16_t config)
{
  s->enableUART(index);
  s->setUART(index, baudrate, config);
}

void VidorUart::end()
{
  s->disableUART(index);
  rxBuffer.clear();
  txBuffer.clear();
}

void VidorUart::flush()
{
  while(txBuffer.available()); // wait until TX buffer is empty
  s->flushUART(index);
}

int VidorUart::available()
{
  return s->availableUART(index);
}

void VidorUart::onInterrupt()
{
  int available = s->availableUART(index);
  while (available-- > 0) {
    rxBuffer.store_char(s->readUART(index));
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
  return s->readUART(index);
}

size_t VidorUart::write(const uint8_t data)
{
  s->writeUART(index, data);
  return 1;
}

/*
size_t Uart::write(const char* data, size_t len)
{
  sercom->writeUART(index, data, len);
  return 1;
}
*/

VidorUart SerialEx(&VD, 0);
