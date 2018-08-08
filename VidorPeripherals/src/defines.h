/*
  This file is part of the VidorPeripherals library.
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

#include "common.h"

#ifndef __DEFINES_VIDOR_H__
#define __DEFINES_VIDOR_H__

#define FPGA_SPI_INTERFACES_COUNT	4
#define FPGA_I2C_INTERFACES_COUNT	4
#define FPGA_UART_INTERFACES_COUNT	8
#define FPGA_ENCODERS_COUNT			11
#define FPGA_NEOPIXEL_COUNT			4
#define FPGA_CAMERA_COUNT			0
#define FPGA_GFX_COUNT				0
#define FPGA_QR_COUNT				0

#define GPIO_NUM_OFFSET				100

#define IRQ_PIN						33

#define NEOPIXEL_PIN_0				A6
#define NEOPIXEL_PIN_1				0
#define NEOPIXEL_PIN_2				7
#define NEOPIXEL_PIN_3				8
#define NEOPIXEL_PIN_4				12

#define NEOPIXEL_PINMUX				5

#endif