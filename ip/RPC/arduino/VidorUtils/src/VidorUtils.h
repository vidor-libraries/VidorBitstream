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

class VidorUtils {
public:
	VidorUtils();

	int begin(bool jumpToApp = true);
	void end();
	void reload();
};

#endif
