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

#ifndef __COMMON_VIDOR_H__
#define __COMMON_VIDOR_H__

#include <stdint.h>
#include <Arduino.h>

#if defined(ARDUINO_SAMD_BETA_MKRVIDOR4000) || defined(ARDUINO_SAMD_MKRVIDOR4000)
static constexpr int digital_to_fpga(int x) {
  return (x < A0 ? x + A6 - A0 + 2 : x - A0 + 1);
};
#else
static constexpr int digital_to_fpga(int x) {
  return x;
}
#endif

#endif
