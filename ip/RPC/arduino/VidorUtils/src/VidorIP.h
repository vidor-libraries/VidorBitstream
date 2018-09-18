/*
  This file is part of the VidorBoot/VidorPeripherals/VidorGraphics library.
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

#ifndef __VIDOR_IP_H__
#define __VIDOR_IP_H__

#include "Arduino.h"
#include <cstdarg>

#define FPGA_UID        0x00000
#define PIO_UID         0xE0C45
#define PWM_UID         0x08A55
#define UART_UID        0x01345
#define I2C_UID         0x15540
#define NEOPIXEL_UID    0xa0894
#define ENC_UID         0x0D645
#define TSPI_UID        0xEBCE5
#define GFX_UID         0xE9422

typedef struct IPInfo {
	int giid;
	int uid;
	int chn;
};

class VidorIP {
public:
	VidorIP() {};
	~VidorIP() {
		deinit();
	}

	virtual int begin() = 0;
	int version();
	int init(int uid, ...);
	int deinit();
	int callback(void(*fn)(void*, int)) {
		cb = fn;
	}

	void(*cb)(void*, int) = NULL;
	IPInfo info;
};

#endif //__VIDOR_IP_H__