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

#ifndef __VIDOR_UTILS_H__
#define __VIDOR_UTILS_H__

#include "LinkedList.h"
#include "defines.h"
#include "VidorMailbox.h"

// Defines for fpga_bitstream_signature section
#define no_data		0xFF, 0xFF, 0xFF, 0xFF, \
					0xFF, 0xFF, 0xFF, 0xFF, \
					0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, \
					0xFF, 0xFF, 0xFF, 0xFF, \
					0x00, 0x00, 0x00, 0x00  \

#define NO_BOOTLOADER		no_data
#define NO_APP				no_data
#define NO_USER_DATA		no_data

__attribute__((weak)) void enableFpgaClock() {}
__attribute__((weak)) void disableFpgaClock() {}

typedef struct IPInfo {
  int giid;
  int uid;
  int chn;
};

class VidorUtils {
public:
	VidorUtils();

	int begin(bool jumpToApp = true);
	void end();
  void reload();
  bool ready();
  uint32_t version();
  static void onInterrupt();

  template <typename... Args> static int discover(IPInfo* info) {
    uint32_t  rpc[32];

    rpc[0] = RPC_CMD(0, 0, 3);
    rpc[1] = info->uid;

    // no pin requested
    rpc[2] = 0;
    int ret = VidorMailbox.sendCommand(rpc, 3+(rpc[2]+3)/4);

    if (ret != -1) {
      info->giid = ((ret >> 24) & 0xFF);
      info->chn =  ((ret >> 12) & 0xFFF);
    }

    return (ret >> 24);
  }

  template <typename... Args> static int discover(IPInfo* info, Args&&... pins) {
    uint32_t  rpc[32];

    rpc[0] = RPC_CMD(0, 0, 3);
    rpc[1] = info->uid;

    int16_t dummy[] = {pins...};
    rpc[2] = sizeof(dummy);

    memcpy(&rpc[3], dummy, sizeof(dummy));

    int ret = VidorMailbox.sendCommand(rpc, 3+(rpc[2]+3)/4);

    if (ret != -1) {
      info->giid = ((ret >> 24) & 0xFF);
      info->chn =  ((ret >> 12) & 0xFFF);
    }

    return (ret >> 24);
  }
};

#endif
