/*
 * TWI/I2C library for Arduino Zero
 * Copyright (c) 2015 Arduino LLC. All rights reserved.
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

extern "C" {
  #include <string.h>
}

#include <Arduino.h>

#include "VidorMailbox.h"
#include "VidorI2C.h"

VidorTwoWire::VidorTwoWire(int index,int _scl,int _sda)
{
  this->idx = index;
  transmissionBegun = false;
  scl = _scl;
  sda = _sda;
  // TODO nuova implementazione FPGA devIdx = FPGA.devIdxGet(FPGA_I2C_DID);
  devIdx = MB_DEV_I2C;
}

int VidorTwoWire::begin(void) {
  int mode;

  // Master Mode
  if (idx < 3) {
    mode = 4;
  }else{
    mode = 5;
  }
  if (sda == -1) {
    goto set_scl;
  }
  if (sda <= 14) {
    sda = sda-0;
    pinMode(sda,INPUT);
    pinMode(sda+140, mode);
  } else {
    sda = sda-A0;
    pinMode(sda,INPUT);
    pinMode(sda+133, mode);
  }

set_scl:
  if (scl == -1) {
    goto i2c_apply;
  }
  if (scl <= 14) {
    scl = scl-0;
    pinMode(scl, INPUT);
    pinMode(scl+140, mode);
  } else {
    scl = scl-A0;
    pinMode(scl, INPUT);
    pinMode(scl+133, mode);
  }

i2c_apply:
  uint32_t rpc[1];

  rpc[0] = MB_CMD(devIdx, idx, 0, 0x01);
  return VidorMailbox.sendCommand(rpc, 1);
}

void VidorTwoWire::begin(uint8_t address) {
  // Slave mode
  // Don't implement right now
}

void VidorTwoWire::setClock(uint32_t baudrate) {
  uint32_t rpc[2];
  rpc[0] = MB_CMD(devIdx, idx, 0, 0x02);
  rpc[1] = baudrate;
  VidorMailbox.sendCommand(rpc, 2);
}

void VidorTwoWire::end() {
  uint32_t rpc[1];

  rpc[0] = MB_CMD(devIdx, idx, 0, 0x03);
  VidorMailbox.sendCommand(rpc, 1);
}

uint8_t VidorTwoWire::requestFrom(uint8_t address, size_t quantity, bool stopBit)
{
  if (quantity == 0) {
    return 0;
  }

  uint32_t rpc[256];
  size_t i;
  uint8_t *ptr;
  int ret;

  rxBuffer.clear();

  rpc[0] = MB_CMD(devIdx, idx, 0, 0x05);
  rpc[1] = address;
  rpc[2] = quantity;
  ret = VidorMailbox.sendCommand(rpc, 3);
  if (ret) {
    return 0;
  }

  for(i=0; i<1+(quantity+3)/4; i++){
    VidorMailbox.read(2+i, &rpc[2+i], 1);
  }

  ptr = (uint8_t*)&rpc[3];
  for(i=0; i<quantity; i++){
    rxBuffer.store_char(ptr[i]);
  }

  return quantity;
}

uint8_t VidorTwoWire::requestFrom(uint8_t address, size_t quantity)
{
  return requestFrom(address, quantity, true);
}

void VidorTwoWire::beginTransmission(uint8_t address) {
  // save address of target and clear buffer
  txAddress = address;
  txBuffer.clear();

  transmissionBegun = true;
}

uint8_t VidorTwoWire::endTransmission(bool stopBit)
{
  uint32_t rpc[256];
  uint8_t *ptr;
  int ret;
  transmissionBegun = false ;

  rpc[0] = MB_CMD(devIdx, idx, 0, 0x08);
  rpc[1] = txAddress;
  rpc[2] = txBuffer.available();

  ptr = (uint8_t*)&rpc[3];
  while (txBuffer.available()) {
    *ptr = txBuffer.read_char();
    ptr++;
  }
  ret = VidorMailbox.sendCommand(rpc, 3+(rpc[2]+3)/4);
  if (ret) {
    return 3;
  }

  return 0;
}

uint8_t VidorTwoWire::endTransmission()
{
  return endTransmission(true);
}

size_t VidorTwoWire::write(uint8_t ucData)
{
  // No writing, without begun transmission or a full buffer
  if ( !transmissionBegun || txBuffer.isFull() )
  {
    return 0 ;
  }

  txBuffer.store_char( ucData ) ;

  return 1 ;
}

size_t VidorTwoWire::write(const uint8_t *data, size_t quantity)
{
  //Try to store all data
  for(size_t i = 0; i < quantity; ++i)
  {
    //Return the number of data stored, when the buffer is full (if write return 0)
    if(!write(data[i]))
      return i;
  }

  //All data stored
  return quantity;
}

int VidorTwoWire::available(void)
{
  return rxBuffer.available();
}

int VidorTwoWire::read(void)
{
  return rxBuffer.read_char();
}

int VidorTwoWire::peek(void)
{
  return rxBuffer.peek();
}

void VidorTwoWire::flush(void)
{
  // Do nothing, use endTransmission(..) to force
  // data transfer.
}

void VidorTwoWire::onReceive(void(*function)(int))
{
  onReceiveCallback = function;
}

void VidorTwoWire::onRequest(void(*function)(void))
{
  onRequestCallback = function;
}

void VidorTwoWire::onService(void)
{
	// I2C slave, not implemented ATM
}

VidorTwoWire WireFPGA0(1,9,10);
VidorTwoWire WireFPGA1(2,11,12);
VidorTwoWire WireFPGA2(3,13,14);
VidorTwoWire WireFPGA3(4,A0,A1);

// Internal pins to control the camera (P1 and P2 on the FPGA)
// TODO: fixme with the actual code
VidorTwoWire WireEx(0, -1, -1);
