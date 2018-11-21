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

VidorUart::VidorUart(int _tx, int _rx, int _cts, int _rts, int _dtr, int _dsr)
{
  tx  = _tx;
  rx  = _rx;
  cts = _cts;
  rts = _rts;
  dtr = _dtr;
  dsr = _dsr;
}

int VidorUart::begin()
{
  if (initialized) {
    return -1;
  }
  if (cts != -1 && rts != -1) {
    if (dtr != -1 && dsr != -1) {
      return init(UART_UID, digital_to_fpga(tx), digital_to_fpga(rx), digital_to_fpga(cts), digital_to_fpga(rts), digital_to_fpga(dtr), digital_to_fpga(dsr));
    } else {
      return init(UART_UID, digital_to_fpga(tx), digital_to_fpga(rx), digital_to_fpga(cts), digital_to_fpga(rts));
    }
  } else {
    return init(UART_UID, digital_to_fpga(tx), digital_to_fpga(rx));
  }
}

void VidorUart::begin(unsigned long baudrate)
{
  begin(baudrate, SERIAL_8N1);
}

void VidorUart::begin(unsigned long baudrate, uint16_t config)
{
  int ret = begin();
  if (ret < 0 && info.giid == -1) {
    return;
  }

  callback(onInterrupt);
  if (!initialized) {
    enableUART();
    initialized = true;
  }
  setUART(baudrate, config);
}

void VidorUart::end()
{
  if (initialized) {
    disableUART();
    initialized = false;
  }
  rxBuffer.clear();
}

void VidorUart::enableUART() {
  uint32_t rpc[1];
  rpc[0] = RPC_CMD(info.giid, info.chn, 2);
  VidorMailbox.sendCommand(rpc, 1);
}

void VidorUart::setUART(int baud, int config) {
  uint32_t rpc[3];
  rpc[0] = RPC_CMD(info.giid, info.chn, 5);
  rpc[1] = baud;
  rpc[2] = config;
  VidorMailbox.sendCommand(rpc, 3);
}

void VidorUart::disableUART() {
  uint32_t rpc[1];
  rpc[0] = RPC_CMD(info.giid, info.chn, 4);
  VidorMailbox.sendCommand(rpc, 1);
}

void VidorUart::flush() {
  uint32_t rpc[1];
  rpc[0] = RPC_CMD(info.giid, info.chn, 11);
  VidorMailbox.sendCommand(rpc, 1);
}

int VidorUart::onInterrupt(void* buf, int n, VidorIP* ip) {
  VidorUart* uart =  (VidorUart*)ip;
  uart->getData();
}

int VidorUart::getData() {
  uint32_t rpc[256];
  rpc[0] = RPC_CMD(info.giid, info.chn, 8);
  int ret = VidorMailbox.sendCommand(rpc, 1);
  if (ret > rxBuffer.availableForStore()) {
    ret = rxBuffer.availableForStore();
  }
  if (ret > 0) {
    rpc[0] = RPC_CMD(info.giid, info.chn, 7);
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

int VidorUart::available()
{
  int ret;

  ret = rxBuffer.available();
  if (ret) {
    return ret;
  }
}

int VidorUart::availableForWrite()
{
  return 1;
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
    len = avail;
  }

  for (size_t i = 0; i < len; i++) {
    data[i] = rxBuffer.read_char();
  }

  return len;
}

size_t VidorUart::write(const uint8_t data)
{
  uint32_t rpc[2];
  rpc[0] = RPC_CMD(info.giid, info.chn, 9);
  rpc[1] = data;
  VidorMailbox.sendCommand(rpc, 2);
  return 1;
}

size_t VidorUart::write(const uint8_t* data, size_t len)
{
  uint32_t rpc[256];
  rpc[0] = RPC_CMD(info.giid, info.chn, 10);
  rpc[1] = len;
  memcpy(&rpc[2], data, len);
  VidorMailbox.sendCommand(rpc, 2+(rpc[1]+3)/4);
  return len;
}

#if FPGA_UART_INTERFACES_COUNT > 0
VidorUart SerialEx(FPGA_NINA_RX, FPGA_NINA_TX, -1, -1, -1, -1);
VidorUart SerialExFlowControl(FPGA_NINA_RX, FPGA_NINA_TX, FPGA_NINA_RTS, FPGA_NINA_CTS, -1, -1);
#if FPGA_UART_INTERFACES_COUNT > 1
VidorUart SerialFPGA0(A0, A1, -1, -1, -1, -1);
#if FPGA_UART_INTERFACES_COUNT > 2
VidorUart SerialFPGA1(A2, A3, A0, A1, -1, -1);
#if FPGA_UART_INTERFACES_COUNT > 3
VidorUart SerialFPGA2(A4, A5, -1, -1, -1, -1);
#if FPGA_UART_INTERFACES_COUNT > 4
VidorUart SerialFPGA3(A6,  0, A4, A5, A3, A2);
#if FPGA_UART_INTERFACES_COUNT > 5
VidorUart SerialFPGA4(1,  2, -1, -1, -1, -1);
#if FPGA_UART_INTERFACES_COUNT > 6
VidorUart SerialFPGA5(3,  4,  1,  2,  0, A6);
#if FPGA_UART_INTERFACES_COUNT > 7
VidorUart SerialFPGA6(5,  6, -1, -1, -1, -1);
#if FPGA_UART_INTERFACES_COUNT > 8
VidorUart SerialFPGA7(7,  8,  5,  6,  4,  3);
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
