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

#ifndef _VIDOR_CAM_H
#define _VIDOR_CAM_H

#include <Arduino.h>

#include "defines.h"
#include "Vidor_GFX.h"


#define CAM_ADDR            0x36
#define CAM_SW_RESET        0x0103
#define CAM_REG_CHIPID_H    0x300A
#define CAM_REG_CHIPID_L    0x300B

#define QR_PT_DET_NUM 12

typedef struct {
    uint32_t xs;
    uint32_t xe;
    uint32_t ys;
    uint32_t ye;
    uint32_t valid;
}sQrPnt, *psQrPnt;

/**
 *
 */
typedef struct{
  uint32_t sts;
  sQrPnt  pt[QR_PT_DET_NUM];
}sQrDet, *psQrDet;

struct regval_list {
  unsigned short  addr;
  unsigned char  data;
};

static struct regval_list cam_640x480[] = {
  {0x0100, 0x00},
  {0x0103, 0x01},
  {0x3034, 0x08},
  {0x3035, 0x41},
  {0x3036, 0x46},
  {0x303c, 0x11},
  {0x3106, 0xf5},
  {0x3821, 0x07},
  {0x3820, 0x41},
  {0x3827, 0xec},
  {0x370c, 0x0f},
  {0x3612, 0x59},
  {0x3618, 0x00},
  {0x5000, 0x06},
  {0x5001, 0x00},
  {0x5002, 0x40},
  {0x5003, 0x08},
  {0x5a00, 0x08},
  {0x3000, 0x00},
  {0x3001, 0x00},
  {0x3002, 0x00},
  {0x3016, 0x08},
  {0x3017, 0xe0},
  {0x3018, 0x44},
  {0x301c, 0xf8},
  {0x301d, 0xf0},
  {0x3a18, 0x00},
  {0x3a19, 0xf8},
  {0x3c01, 0x80},
  {0x3b07, 0x0c},
  {0x380c, 0x07},
  {0x380d, 0x68},
  {0x380e, 0x03},
  {0x380f, 0xd8},
  {0x3814, 0x31},
  {0x3815, 0x31},
  {0x3708, 0x64},
  {0x3709, 0x52},
  {0x3808, 0x02},
  {0x3809, 0x80},
  {0x380a, 0x01},
  {0x380b, 0xe0},
  {0x3800, 0x00},
  {0x3801, 0x00},
  {0x3802, 0x00},
  {0x3803, 0x00},
  {0x3804, 0x0a},
  {0x3805, 0x3f},
  {0x3806, 0x07},
  {0x3807, 0xa1},
  {0x3811, 0x08},
  {0x3813, 0x02},
  {0x3630, 0x2e},
  {0x3632, 0xe2},
  {0x3633, 0x23},
  {0x3634, 0x44},
  {0x3636, 0x06},
  {0x3620, 0x64},
  {0x3621, 0xe0},
  {0x3600, 0x37},
  {0x3704, 0xa0},
  {0x3703, 0x5a},
  {0x3715, 0x78},
  {0x3717, 0x01},
  {0x3731, 0x02},
  {0x370b, 0x60},
  {0x3705, 0x1a},
  {0x3f05, 0x02},
  {0x3f06, 0x10},
  {0x3f01, 0x0a},
  {0x3a08, 0x01},
  {0x3a09, 0x27},
  {0x3a0a, 0x00},
  {0x3a0b, 0xf6},
  {0x3a0d, 0x04},
  {0x3a0e, 0x03},
  {0x3a0f, 0x58},
  {0x3a10, 0x50},
  {0x3a1b, 0x58},
  {0x3a1e, 0x50},
  {0x3a11, 0x60},
  {0x3a1f, 0x28},
  {0x4001, 0x02},
  {0x4004, 0x02},
  {0x4000, 0x09},
  {0x4837, 0x24},
  {0x4050, 0x6e},
  {0x4051, 0x8f},
  {0x503d, 0x00},
  {0x0100, 0x01},
};

class VidorQR {

  public:
    sQrDet qr;
    VidorQR(void);
    void begin() {enable(true);}
    void end() {enable(false);}
    void setMode(uint8_t mode);
    void setThr(uint8_t thr);
    int readQRCode(void);
  private:
    void enable(uint8_t on);
};


class VidorCamera {

  public:
    Vidor_GFX vgfx;
    VidorQR qrrec;
    VidorCamera();
    int begin();
    int end() {setPower(false);}
    int enableStream();
    int disableStream();
    int modelDetect(void);
  protected:
    int setPower(bool on);
    int sensor_init(void);
    int write_array(struct regval_list *regs, int array_size);
    int set_sw_standby(int standby);
    int set_virtual_channel(int channel);
    int read(uint8_t address, uint16_t reg, uint8_t* data, uint8_t len);
    int write(uint8_t address, uint16_t reg, uint8_t data);

};

#endif //_VIDOR_CAM_H
