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
#include "VidorUtils.h"

#define FPGA_UID        0x00000
#define PIO_UID         0xE0C45
#define PWM_UID         0x08A55
#define UART_UID        0x01345
#define I2C_UID         0x15540
#define NEOPIXEL_UID    0xa0894
#define ENC_UID         0x0D645
#define TSPI_UID        0xEBCE5
#define GFX_UID         0xE9422
#define QR_UID  		0xA2C68

class VidorIP {
public:
	VidorIP() {};
	~VidorIP() {
		deinit();
	}

	virtual int begin() = 0;
	int version();
	void addToList();
	IPInfo info;

	template <typename... Args> int init(int uid, Args&&... pins) {
		// MUST be called after FPGA.init()
		this->info.uid = uid;
		// scan list and replace self with matching uid 
		int ret = VidorUtils::discover(&info, pins...);
		if (ret >= 0) {
			// TODO: here a call should exist to multiplex and lock pins for a specific giid/chn
			// rpc[0] = RPC_CMD(info.giid, info.chn, 4);
			// int ret = VidorMailbox.sendCommand(rpc, 1);
			addToList();
		}
		return ret;
	}

	int deinit();
	int callback(int(*fn)(void*, int, VidorIP*)) {
		cb = fn;
	}

	int(*cb)(void*, int, VidorIP*) = NULL;
};

#endif //__VIDOR_IP_H__