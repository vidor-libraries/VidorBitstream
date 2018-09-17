/*
  This file is part of the VidorBoot library.
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

// Only needed by assemble_library script
#include "common.h"

#define GPIO_NUM_OFFSET                         100
#define IRQ_PIN                                 33

// NINA signals
#define SPIWIFI              SPIEx
#define FPGA_NINA_MOSI       (64 + 19)
#define FPGA_NINA_MISO       (64 + 20)
#define FPGA_NINA_SCK        (64 + 18)
#define FPGA_NINA_GPIO0      (64 + 10)   // WM_PIO27 -> NiNa GPIO0 -> FPGA N9
#define FPGA_SPIWIFI_RESET   (64 + 0)    // WM_RESET -> NiNa RESETN -> FPGA R1
#define FPGA_SPIWIFI_ACK     (64 + 5)    // WM_PIO7  -> NiNa GPIO33 -> FPGA P6
#define FPGA_SPIWIFI_SS      (64 + 11)   // WM_PIO28 -> NiNa GPIO5 -> FPGA N11
