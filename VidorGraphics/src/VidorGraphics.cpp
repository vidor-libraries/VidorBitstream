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

#include "VidorGraphics.h"

VidorGraphics FPGA;

#if 1
__attribute__ ((used, section(".fpga_bitstream_signature")))
const unsigned char signatures[4096] = {
	//#include "signature.ttf"
	NO_BOOTLOADER,

	0x00, 0x00, 0x08, 0x00,
	0x0c, 0x72, 0x06, 0x00,
	0x0f, 0xa8, 0xdc, 0xb2, 0xa1, 0x96, 0x5b, 0x03, 0xde, 0xba, 0x6c, 0xb0, 0x89, 0x2d, 0x9a, 0x12, 0x9d, 0xd0, 0xf9, 0x98, 0x16, 0x83, 0xaa, 0x1f, 0x10, 0x1d, 0xdb, 0x0b, 0x98, 0x26, 0x78, 0xe0,
	0x01, 0x00, 0x00, 0x00,		/* TAG 1 == USER BITSTREAM */
	0x00, 0x00, 0x00, 0x00,		// Don't force

	NO_USER_DATA,
};
__attribute__ ((used, section(".fpga_bitstream")))
const unsigned char bitstream[] = {
	#include "app.ttf"
};
#endif

void VidorGraphics::onInterrupt() {
	// TODO: do be implemented
}

#if 0
// This strong implementation allows booting the FPGA without external intervention
extern "C" void startFPGA() {
	FPGA.begin();
}
#endif