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

	return ret;
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
