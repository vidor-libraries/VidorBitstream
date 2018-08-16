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

#include "utility/jtag.h"

#include "VidorMailbox.h"

#define MB_BASE 0x00000000

VidorMailboxClass::VidorMailboxClass() {}

int VidorMailboxClass::begin()
{
	int ret = jtagInit();
	mbPinSet();

	return (ret == 0);
}

void VidorMailboxClass::end()
{
	jtagDeinit();
}

void VidorMailboxClass::reload()
{
	jtagReload();
}

int VidorMailboxClass::sendCommand(uint32_t data[], size_t len)
{
	return mbCmdSend(data, len);
}

int VidorMailboxClass::sendEvent(uint32_t data[], size_t len)
{
	return mbEveSend(data, len);
}

int VidorMailboxClass::read(uint32_t address, uint32_t data[], size_t len)
{
	return mbRead(MB_BASE + address, (uint8_t*)data, len);
}

int VidorMailboxClass::write(uint32_t address, const uint32_t data[], size_t len)
{
	return mbWrite(MB_BASE + address, (const uint8_t*)data, len);
}

VidorMailboxClass VidorMailbox;
