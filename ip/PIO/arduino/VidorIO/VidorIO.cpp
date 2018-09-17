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

#include "VidorIO.h"

// redefines C-scope pin extended functions (non-weak implementation)

int VidorIO::period = -1;

extern "C" {
void pinModeExtended( uint32_t ulPin, uint32_t ulMode ) {
	// TODO: add some checks if ulPin makes sense?
	// ulMode can be INPUT, INPUT_PULLUP or OUTPUT
	VidorIO::pinMode(ulPin - GPIO_NUM_OFFSET, ulMode);
}

void digitalWriteExtended( uint32_t ulPin, uint32_t ulVal ) {
	// No need to check if the pin is alredy in GPIO mode
	VidorIO::digitalWrite(ulPin - GPIO_NUM_OFFSET, ulVal);
}

int digitalReadExtended( uint32_t ulPin ) {
	// No need to check if the pin is alredy in GPIO mode
	return VidorIO::digitalRead(ulPin - GPIO_NUM_OFFSET);
}

void analogWriteExtended( uint32_t ulPin, uint32_t ulVal ) {
	// check if mux for the pin is already in PWM mode and eventually change it
	pinModeExtended(ulPin, 3);
	// set the PWM duty
	return VidorIO::analogWrite(ulPin - GPIO_NUM_OFFSET, ulVal);
}
}