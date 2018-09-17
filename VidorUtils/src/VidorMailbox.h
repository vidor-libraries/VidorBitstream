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

#ifndef __VIDOR_MAILBOX_H__
#define __VIDOR_MAILBOX_H__

#include <Arduino.h>

class VidorMailboxClass {
public:
	VidorMailboxClass();

	int begin();
	void end();
	void reload();

	int sendCommand(const uint32_t data[], size_t len);
	int sendEvent(const uint32_t data[], size_t len);
	int read(uint32_t address, uint32_t data[], size_t len);
	int write(uint32_t address, const uint32_t data[], size_t len);
};

extern VidorMailboxClass VidorMailbox;

#endif
