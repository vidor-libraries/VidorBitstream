/*
  This file is part of the VidorPeripherals library.
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
	0xA9, 0x6F, 0x1F, 0x00,
	0x6f, 0x90, 0x25, 0x62, 0xb4, 0xec, 0xae, 0x0a, 0xa7, 0x28, 0x22, 0xc6, 0x91, 0xbf, 0x61, 0x65, 0xef, 0x0d, 0x9d, 0x7c, 0x04, 0x19, 0x82, 0x3b, 0x0a, 0xf1, 0x4e, 0x00, 0x00, 0xff, 0xf0, 0x0f,
	0x01, 0x00, 0x00, 0x00,		/* TAG 1 == USER BITSTREAM */
	0x00, 0x00, 0x00, 0x00,		// Don't force

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