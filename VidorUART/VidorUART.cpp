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
  idx = _idx;
  tx  = _tx;
  rx  = _rx;
  cts = _cts;
  rts = _rts;
  dtr = _dtr;
  dsr = _dsr;
  // TODO nuova implementazione FPGA giid = FPGA.instance(....);
  devIdx = MB_DEV_UART;
}

void VidorUart::begin(unsigned long baudrate)
{
  begin(baudrate, SERIAL_8N1);
}

void VidorUart::begin(unsigned long baudrate, uint16_t config)
{
  enableUART(tx, rx);
  setUART(baudrate, config);
}

void VidorUart::end()
{
  disableUART();
  rxBuffer.clear();
  txBuffer.clear();
}

void VidorUart::enableUART(int tx, int rx) {
  if (tx == -1) {
    VidorIO::pinMode(FPGA_NINA_RX, 4);
  } else if (tx <= 14) {
    tx = tx-0 ;
    pinModeExtended(tx+140, 4);
  } else {
    tx = tx-A0;
    pinModeExtended(tx+133, 4);
  }
  if (rx == -1) {
    VidorIO::pinMode(FPGA_NINA_TX, INPUT);
  } else if (rx <= 14) {
    rx = rx-0 ;
    pinModeExtended(rx+140, INPUT);
  } else {
    rx = rx-A0;
    pinModeExtended(rx+133, INPUT);
  }
  uint32_t rpc[2];
  rpc[0] = MB_CMD(devIdx, idx, 0, 0x01);
  rpc[1] = 0; // TX-RX only
  VidorMailbox.sendCommand(rpc, 2);
}

void VidorUart::setUART(int baud, int config) {
  uint32_t rpc[3];
  rpc[0] = MB_CMD(devIdx, idx, 0, 0x02);
  rpc[1] = baud;
  rpc[2] = config;
  VidorMailbox.sendCommand(rpc, 3);
}

void VidorUart::disableUART() {
  uint32_t rpc[1];
  rpc[0] = MB_CMD(devIdx, idx, 0, 0x03);
  VidorMailbox.sendCommand(rpc, 1);
}

void VidorUart::flush() {
  uint32_t rpc[1];
  while(txBuffer.available()); // wait until TX buffer is empty
  rpc[0] = MB_CMD(devIdx, idx, 0, 0x09);
  VidorMailbox.sendCommand(rpc, 1);
}

int VidorUart::available()
{
  int ret;

  ret = rxBuffer.available();
  if (ret) {
    return ret;
  }

  uint32_t rpc[256];
  rpc[0] = MB_CMD(devIdx, idx, 0, 0x06);
  ret = VidorMailbox.sendCommand(rpc, 1);
  if (ret > 0) {
    rpc[0] = MB_CMD(devIdx, idx, 0, 0x05);
    rpc[1] = ret;
    ret = VidorMailbox.sendCommand(rpc, 2);
    VidorMailbox.read(2, &rpc[2], 1+(ret+3)/4);
    uint8_t* data = (uint8_t*)&rpc[2];
    for (int i = 0; i < ret; i++) {
      rxBuffer.store_char(data[i]);
    }
  }

  return ret;
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
  if (!available()) {
    return -1;
  }

  return rxBuffer.read_char();
}

int VidorUart::read(uint8_t* data, size_t len)
{
  int avail = available();

  if (!avail) {
    return -1;
  }

  if ((int)len > avail) {
    avail = len;
  }

  for (size_t i = 0; i < len; i++) {
    data[i] = rxBuffer.read_char();
  }

  return len;
}

size_t VidorUart::write(const uint8_t data)
{
  uint32_t rpc[2];
  rpc[0] = MB_CMD(devIdx, idx, 0, 0x07);
  rpc[1] = data;
  VidorMailbox.sendCommand(rpc, 2);
  return 1;
}

size_t VidorUart::write(const uint8_t* data, size_t len)
{
  uint32_t rpc[256];
  rpc[0] = MB_CMD(devIdx, idx, 0, 0x08);
  rpc[1] = len;
  memcpy(&rpc[2], data, len);
  VidorMailbox.sendCommand(rpc, 2+(rpc[1]+3)/4);
  return len;
}

int VidorUart::enableFlowControl(void)
{
  if(rts>=0 && cts>=0 && dtr>=0 && dsr>=0 ) {
    enableUART(rts, cts);
    enableUART(dtr, dsr);
    return 1;
  }
  return 0;
}


#if FPGA_UART_INTERFACES_COUNT > 0
VidorUart SerialEx(   0,  -1, -1, -1, -1, -1, -1);
#if FPGA_UART_INTERFACES_COUNT > 1
VidorUart SerialFPGA0(1,  A0, A1, -1, -1, -1, -1);
#if FPGA_UART_INTERFACES_COUNT > 2
VidorUart SerialFPGA1(2,  A2, A3, A0, A1, -1, -1);
#if FPGA_UART_INTERFACES_COUNT > 3
VidorUart SerialFPGA2(3,  A4, A5, -1, -1, -1, -1);
#if FPGA_UART_INTERFACES_COUNT > 4
VidorUart SerialFPGA3(4,  A6,  0, A4, A5, A3, A2);
#if FPGA_UART_INTERFACES_COUNT > 5
VidorUart SerialFPGA4(5,   1,  2, -1, -1, -1, -1);
#if FPGA_UART_INTERFACES_COUNT > 6
VidorUart SerialFPGA5(6,   3,  4,  1,  2,  0, A6);
#if FPGA_UART_INTERFACES_COUNT > 7
VidorUart SerialFPGA6(7,   5,  6, -1, -1, -1, -1);
#if FPGA_UART_INTERFACES_COUNT > 8
VidorUart SerialFPGA7(8,   7,  8,  5,  6,  4,  3);
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
