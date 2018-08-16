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

#include "VidorUtils.h"

__attribute__((weak)) void enableFpgaClock() {}
__attribute__((weak)) void disableFpgaClock() {}

VidorUtils::VidorUtils() {}

int VidorUtils::begin(bool jumpToApp)
{
	// Start clocking the FPGA; this function is declared weak and can be overridden
	// with a custom implementation (or can be left untouched if FPGA is clocked internally)
	enableFpgaClock();

	int ret = jtagInit();
	mbPinSet();

	if (ret == 0 && jumpToApp) {
		uint32_t ptr[1];
		ptr[0] = 0 | 3;
		mbEveSend(ptr, 1);
	}

	return (ret == 0);
}

void VidorUtils::end()
{
	disableFpgaClock();
	jtagDeinit();
}

void VidorUtils::reload()
{
	jtagReload();
}
