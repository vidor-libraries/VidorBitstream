/*
  This file is part of the VidorGraphics library.
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

#define FPGA_SPI_INTERFACES_COUNT	1
#define FPGA_I2C_INTERFACES_COUNT	2
#define FPGA_UART_INTERFACES_COUNT	1
#define FPGA_ENCODERS_COUNT			11
#define FPGA_NEOPIXEL_COUNT			11
#define FPGA_CAMERA_COUNT			1
#define FPGA_GFX_COUNT				1
#define FPGA_QR_COUNT				1

#define GPIO_NUM_OFFSET				100

#define IRQ_PIN						33

#define NEOPIXEL_PIN_0				A0
#define NEOPIXEL_PIN_1				A1
#define NEOPIXEL_PIN_2				A2
#define NEOPIXEL_PIN_3				A3
#define NEOPIXEL_PIN_4				A4
#define NEOPIXEL_PIN_5				A5
#define NEOPIXEL_PIN_6				A6
#define NEOPIXEL_PIN_7				0
#define NEOPIXEL_PIN_8				1
#define NEOPIXEL_PIN_9				2
#define NEOPIXEL_PIN_10				3

#define NEOPIXEL_PINMUX				4
#define NEOPIXEL_FB_INDEX			1

// NINA signals
#define SPIWIFI              SPIEx
#define SerialNina           SerialEx
#define FPGA_NINA_TX         (64 + 16)
#define FPGA_NINA_RX         (64 + 15)
#define FPGA_NINA_MOSI       (64 + 19)
#define FPGA_NINA_MISO       (64 + 20)
#define FPGA_NINA_SCK        (64 + 18)
#define FPGA_NINA_GPIO0      (64 + 10)   // WM_PIO27 -> NiNa GPIO0 -> FPGA N9
#define FPGA_SPIWIFI_RESET   (64 + 0)    // WM_RESET -> NiNa RESETN -> FPGA R1
#define FPGA_SPIWIFI_ACK     (64 + 5)    // WM_PIO7  -> NiNa GPIO33 -> FPGA P6
#define FPGA_SPIWIFI_SS      (64 + 11)   // WM_PIO28 -> NiNa GPIO5 -> FPGA N11

#endif
