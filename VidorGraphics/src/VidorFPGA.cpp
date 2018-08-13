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

#include "VidorFPGA.h"

VidorFPGA FPGA;

#if 1
__attribute__ ((used, section(".fpga_bitstream_signature")))
const unsigned char signatures[4096] = {
	//#include "signature.ttf"
	NO_BOOTLOADER,

	0x00, 0x00, 0x08, 0x00,
	0xbc, 0x72, 0x06, 0x00,
	0x1e, 0x37, 0xb5, 0xb6, 0x55, 0xe0, 0x0c, 0xe3, 0x3f, 0xcc, 0x73, 0xd8, 0xa9, 0x39, 0x98, 0x70, 0x69, 0x23, 0xd1, 0x48, 0x06, 0xab, 0x37, 0x55, 0x38, 0x11, 0x0a, 0x4f, 0xdb, 0x97, 0x36, 0x8a,
	0x01, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, // force flag, change to 1 if needed

	NO_USER_DATA,
};
__attribute__ ((used, section(".fpga_bitstream")))
const unsigned char bitstream[] = {
	#include "app.ttf"
};
#endif

void VidorFPGA::onInterrupt() {
	// TODO: do be implemented
}

#if 0
// This strong implementation allows booting the FPGA without external intervention
extern "C" void startFPGA() {
	FPGA.begin();
}
#endif