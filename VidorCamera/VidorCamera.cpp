/*
  This file is part of the VidorGraphics library.
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

#include "VidorI2C.h"
#include "VidorMailbox.h"

#include "VidorCamera.h"

VidorCamera::VidorCamera(){
}

VidorQR::VidorQR(void){

}

int VidorCamera::begin() {
  int ret=0;
  WireEx.begin();
  vgfx.fillRect(0, 0, 640, 480,0,0);
  ret = setPower(true);
  return ret;
}

int VidorCamera::setPower(bool on) {
  int ret = 0;
  if (on) {
    ret = sensor_init();
  } else {
    ret = set_sw_standby(1);
  }
  return ret;
}

int VidorCamera::enableStream() {
  int ret;
  ret = write(CAM_ADDR, 0x4202, 0x00);
  if (ret < 0) {
    return ret;
  }
  return write(CAM_ADDR, 0x300D, 0x00);
}

int VidorCamera::disableStream(){
  int ret;
  ret = write(CAM_ADDR, 0x4202, 0x0f);
  if (ret < 0) {
    return ret;
  }
  return write(CAM_ADDR, 0x300D, 0x01);
}

int VidorCamera::modelDetect(void) {
  unsigned char readel;
  int ret;
  ret = write(CAM_ADDR, CAM_SW_RESET, 0x01);
  if (ret < 0) {
    return ret;
  }
  ret = read(CAM_ADDR, CAM_REG_CHIPID_H, &readel, 1);
  if (ret < 0) {
    return ret;
  }
  if (readel != 0x56) {
    return -1;
  }
  ret = read(CAM_ADDR, CAM_REG_CHIPID_L, &readel, 1);
  if (ret < 0) {
    return ret;
  }
  if (readel != 0x47) {
    return -1;
  }
  return write(CAM_ADDR, CAM_SW_RESET, 0x00);
}

int VidorCamera::write(uint8_t address, uint16_t reg, uint8_t data) {
  int ret;
  WireEx.beginTransmission(address);

  ret=WireEx.write((uint8_t)(reg>>8));
  if(ret==0){
  	  return ret;
  }
  ret=WireEx.write((uint8_t)(reg));
  if(ret==0){
  	  return ret;
  }
  ret=WireEx.write(data);
  WireEx.endTransmission();
  return ret;
}

int VidorCamera::read(uint8_t address, uint16_t reg, uint8_t* data, uint8_t len) {
  WireEx.beginTransmission(address);
  WireEx.write((uint8_t)(reg>>8));
  WireEx.write((uint8_t)(reg));
  WireEx.endTransmission();
  WireEx.requestFrom(address, len);
  if (len <= WireEx.available()) {
    int i;
    for (i=0; i<len; i++) {
      data[i] = WireEx.read();
    }
    return len;
  }
  return 0;
}

int VidorCamera::sensor_init(void) {
  int ret;
  unsigned char resetval, rdval;
  ret = read(CAM_ADDR, 0x0100, &rdval, 1);
  if (ret < 0) {
    return ret;
  }
  ret = write_array(cam_640x480, (sizeof(cam_640x480) / sizeof(struct regval_list)));
  if (ret == 0) {
    return ret;
  }
  ret = set_virtual_channel( 0);
  if (ret == 0) {
    return ret;
  }
  ret = read(CAM_ADDR, 0x0100, &resetval, 1);
  if (ret < 0) {
    return ret;
  }
  if (!(resetval & 0x01)) {
    ret = write(CAM_ADDR, 0x0100, 0x01);
    if (ret == 0) {
      return ret;
    }
  }
  ret=write(CAM_ADDR, 0x4800, 0x04);
  return ret;
}

int VidorCamera::write_array(struct regval_list *regs, int array_size) {
  int i, ret;

  for (i = 0; i < array_size; i++) {
    ret = write(CAM_ADDR, regs[i].addr, regs[i].data);
    if (ret == 0) {
      return ret;
    }
  }
  return ret;
}


int VidorCamera::set_sw_standby(int standby) {
  int ret;
  unsigned char rdval;

  ret = read(CAM_ADDR, 0x0100, &rdval, 1);
  if (ret < 0) {
    return ret;
  }
  if (standby) {
    rdval &= ~0x01;
  } else {
    rdval |= 0x01;
  }
  return write(CAM_ADDR, 0x0100, rdval);
}

int VidorCamera::set_virtual_channel(int channel) {
  unsigned char channel_id;
  int ret;
  ret = read(CAM_ADDR, 0x4814, &channel_id, 1);
  if (ret < 0) {
    return ret;
  }
  channel_id &= ~(3 << 6);

  return write(CAM_ADDR, 0x4814, channel_id | (channel << 6));
}

void VidorQR::enable(uint8_t on) {
  uint32_t rpc[2];
  rpc[0] = MB_CMD(MB_DEV_QR, 0, 0, 1);
  rpc[1] = on;
  VidorMailbox.sendCommand(rpc, 2);
}

void VidorQR::setMode(uint8_t mode) {
  uint32_t rpc[2];
  rpc[0] = MB_CMD(MB_DEV_QR, 0, 0, 2);
  rpc[1] = mode;
  VidorMailbox.sendCommand(rpc, 2);
}

int VidorQR::readQRCode(void){
  uint32_t rpc[1];
  rpc[0] = MB_CMD(MB_DEV_QR, 0, 0, 3);
  int ret = VidorMailbox.sendCommand(rpc, 1);
  if (ret) {
    return 0;
  } else {
    VidorMailbox.read(2, (uint32_t*)&qr, (sizeof(sQrDet) + 3) / 4);
    return 1;
  }
}

void VidorQR::setThr(uint8_t thr){
  uint32_t rpc[2];
  rpc[0] = MB_CMD(MB_DEV_QR, 0, 0, 4);
  rpc[1] = thr;
  VidorMailbox.sendCommand(rpc, 2);
}
