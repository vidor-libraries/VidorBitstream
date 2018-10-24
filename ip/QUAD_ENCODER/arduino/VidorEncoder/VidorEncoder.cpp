/*
  This file is part of the VidorPeripherals/VidorGraphics library.
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

#include "VidorMailbox.h"
#include "VidorFPGA.h"
#include "VidorEncoder.h"

VidorEncoder::VidorEncoder(int index){
  idx = index;
  // TODO nuova implementazione FPGA devIdx = FPGA.devIdxGet(FPGA_ENCODERS_DID);
  devIdx = MB_DEV_ENC;
}

void VidorEncoder::write(int32_t p){
	offset += read() - p;
}

int32_t VidorEncoder::read(){
	uint32_t rpc[1];
	rpc[0] = MB_CMD(devIdx, idx, 0, 0x01); // TODO move index from sub to chn
	return (VidorMailbox.sendCommand(rpc, 1) - offset);
}
