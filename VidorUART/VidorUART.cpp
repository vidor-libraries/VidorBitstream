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

VidorUart::VidorUart(VidorBase *_s, int _idx,int _tx,int _rx,int _cts,int _rts,int _dtr,int _dsr)
{
  s = _s;
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
  s->enableUART(tx,rx);
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
int VidorUart::enableFlowControl(void){
  if(rts>=0 && cts>=0 && dtr>=0 && dsr>=0 ){
	  s->enableUART(rts,cts);
	  s->enableUART(dtr,dsr);
	return 1;
  }
  return 0;
}


VidorUart SerialFPGA0(&VD, 0,A0,A1,-1,-1,-1,-1);
VidorUart SerialFPGA1(&VD, 1,A2,A3,A0,A1,-1,-1);
VidorUart SerialFPGA2(&VD, 2,A4,A5,-1,-1,-1,-1);
VidorUart SerialFPGA3(&VD, 3,A6,0,A4,A5,A3,A2);
VidorUart SerialFPGA4(&VD, 4,1,2,-1,-1,-1,-1);
VidorUart SerialFPGA5(&VD, 5,3,4,1,2,0,A6);
VidorUart SerialFPGA6(&VD, 6,5,6,-1,-1,-1,-1);
VidorUart SerialFPGA7(&VD, 7,7,8,5,6,4,3);

