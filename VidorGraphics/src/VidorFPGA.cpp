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
	#include "signature.h"
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