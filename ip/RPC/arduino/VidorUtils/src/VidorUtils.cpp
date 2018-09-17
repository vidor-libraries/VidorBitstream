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

#include "VidorMailbox.h"

#include "VidorUtils.h"

VidorUtils::VidorUtils() {}

int VidorUtils::begin(bool jumpToApp)
{
	// Start clocking the FPGA; this function is declared weak and can be overridden
	// with a custom implementation (or can be left untouched if FPGA is clocked internally)
	enableFpgaClock();

	int ret = VidorMailbox.begin();

	if (ret == 1 && jumpToApp) {
		uint32_t evt[1];
		evt[0] = 0 | 3;
		VidorMailbox.sendEvent(evt, 1);
	}

	attachInterrupt(IRQ_PIN, VidorFPGA::onInterrupt, FALLING);

	discover();

	return ret;
}

int VidorUtils::discover() {
	uint32_t  buf[256];
	uint32_t  rpc[1];

	rpc[0] = RPC_CMD(0, 0, 2);
  	howMany = VidorMailbox.sendCommand(rpc, 1);

  	if (howMany == -1) {
  		return -1;
  	}

  	VidorMailbox.read(2, buf, howMany);

	for (int i = 0; i<howMany; i++) {
		VidorIP* ip = new VidorIP();
		ip->uid = buf[i] & 0xFFFF;
		ip->availableChannels = (buf[i] >> 20) & 0xFFF;
		IPList.append(ip);
	}
}

void VidorUtils::onInterrupt() {

	// Call VidorMailbox to retrieve the uid
	int uid = VidorMailbox.sendCommand(GET_IRQ_SOURCE);

	for (int i = 0; i < IPList.size(); i++) {
		VidorIP* ip = IPList.get(i);
		if (ip->uid == uid && ip->cb != NULL) {
			uint8_t data[256];
			int ret = VidorMailbox.sendCommand(GET_IRQ_DATA, data);
			ip->cb(data, ret);
			break;
		}
	}
}

bool VidorUtils::ready() {
	return (version() != 0) &&  (version() != 0xFFFFFFFF);
}

uint32_t VidorUtils::version() {
	uint32_t rpc[1];
	uint32_t ver;

	rpc[0] = 0 | 1;
	ver = VidorMailbox.sendCommand(rpc, 1);
	return ver;
}

void VidorUtils::end()
{
	VidorMailbox.end();
	disableFpgaClock();
}

void VidorUtils::reload()
{
	VidorMailbox.reload();
}
