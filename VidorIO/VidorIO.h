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
#include "defines.h"

#ifndef __VIDOR_IO_H__
#define __VIDOR_IO_H__

extern "C" {
	void pinModeExtended( uint32_t ulPin, uint32_t ulMode );
	void digitalWriteExtended( uint32_t ulPin, uint32_t ulVal );
	int digitalReadExtended( uint32_t ulPin );
	void analogWriteExtended( uint32_t pin, uint32_t value );
}

class VidorIO {

public:
	static void pinMode(uint32_t pin, uint32_t mode) {
		uint32_t rpc[256];
		rpc[0] = MB_CMD(MB_DEV_GPIO, 0, 0, 0x01);
		rpc[1] = pin;

		switch (mode) {
			case OUTPUT:
				rpc[2] = 2;
				break;
			case INPUT:
				rpc[2] = 1;
				break;
			default:
				rpc[2] = mode;
		}
		VidorMailbox.sendCommand(rpc, 3);
	}

	static void digitalWrite(uint32_t pin, uint32_t mode) {
		uint32_t rpc[256];
		rpc[0] = MB_CMD(MB_DEV_GPIO, 0, 0, 0x02);
		rpc[1] = pin;
		rpc[2] = mode;
		VidorMailbox.sendCommand(rpc, 3);
	}

	static int digitalRead(uint32_t pin) {
		uint32_t rpc[256];
		rpc[0] = MB_CMD(MB_DEV_GPIO, 0, 0, 0x03);
		rpc[1] = pin;
		return VidorMailbox.sendCommand(rpc, 2);
	}

	static int period;

	static void analogWriteResolution(int bits, int frequency) {

		uint32_t rpc[256];
		period = 2 << bits;
		int prescaler = (2 * F_CPU / frequency) / period;

		rpc[0] = MB_CMD(MB_DEV_GPIO, 0, 0, 0x04);
		rpc[1] = prescaler;
		rpc[2] = period;
		VidorMailbox.sendCommand(rpc, 3);
	}

	static void analogWrite(uint32_t pin, uint32_t mode) {

		uint32_t rpc[256];

		if (period == -1) {
			// sane default
			analogWriteResolution(8, 490);
		}

		rpc[0] = MB_CMD(MB_DEV_GPIO, 0, 0, 0x05);
		rpc[1] = pin;
		rpc[2] = mode;
		rpc[3] = period - mode;
		VidorMailbox.sendCommand(rpc, 4);
	}

	/* I2C functions moved*/
	/* UART functions moved */
	/* SPI functions moved */
  private:
//TODO    uint8_t devIdx;
};

#endif
