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


/**
 * FPGA DEVICE IDs
 */
#define FPGA_GPIO_DID       0x00000100
#define FPGA_GFX_DID        0x00000200
#define FPGA_SF_DID         0x00000300
#define FPGA_I2C_DID        0x00000400
#define FPGA_SPI_DID        0x00000500
#define FPGA_UART_DID       0x00000600
#define FPGA_QR_DID         0x00000700
#define FPGA_SDRAM_DID      0x00000800
#define FPGA_NEOPIXEL_DID   0x00000900
#define FPGA_ENCODERS_DID   0x00000A00
#define FPGA_REG_DID        0x00000B00

static inline const char* Did2Str(uint32_t did)
{
  switch (did) {
  case FPGA_GPIO_DID    : return "GPIO";
  case FPGA_GFX_DID     : return "GFX";
  case FPGA_SF_DID      : return "SF";
  case FPGA_I2C_DID     : return "I2C";
  case FPGA_SPI_DID     : return "SPI";
  case FPGA_UART_DID    : return "UART";
  case FPGA_QR_DID      : return "QR";
  case FPGA_SDRAM_DID   : return "SDRAM";
  case FPGA_NEOPIXEL_DID: return "NP";
  case FPGA_ENCODERS_DID: return "ENC";
  case FPGA_REG_DID     : return "REG";
  }
  return "---";
}


/*
#define MB_DEV_SF       0x01000000
#define MB_DEV_GPIO     0x02000000
#define MB_DEV_GFX      0x03000000
#define MB_DEV_I2C      0x04000000
#define MB_DEV_SPI      0x05000000
#define MB_DEV_UART     0x06000000
#define MB_DEV_QR       0x07000000
#define MB_DEV_SDRAM    0x08000000
#define MB_DEV_NP       0x09000000
#define MB_DEV_ENC      0x0A000000
#define MB_DEV_REG      0x0B000000
*/
#define MB_DEV_SF       0x01
#define MB_DEV_GPIO     0x02
#define MB_DEV_GFX      0x03
#define MB_DEV_I2C      0x04
#define MB_DEV_SPI      0x05
#define MB_DEV_UART     0x06
#define MB_DEV_QR       0x07
#define MB_DEV_SDRAM    0x08
#define MB_DEV_NP       0x09
#define MB_DEV_ENC      0x0A
#define MB_DEV_REG      0x0B

#define MB_DID_OFS 24
#define MB_SUB_OFS 16
#define MB_CHN_OFS  8
#define MB_CMD_OFS  0

#define MB_CMD(did, sub, chn, cmd) ((did==0xFF)?0: \
                                    (((did)&0xFF)<<MB_DID_OFS) | \
                                    (((sub)&0xFF)<<MB_SUB_OFS) | \
                                    (((chn)&0xFF)<<MB_CHN_OFS) | \
                                    (((cmd)&0xFF)<<MB_CMD_OFS) )

static const char* configToString(uint32_t cod)
{
	switch(cod  >> MB_DID_OFS ){
		case MB_DEV_SF: 	return "MB_DEV_SF";
		case MB_DEV_GPIO: 	return "MB_DEV_GPIO";
		case MB_DEV_GFX: 	return "MB_DEV_GFX";
		case MB_DEV_I2C: 	return "MB_DEV_I2C";
		case MB_DEV_SPI: 	return "MB_DEV_SPI";
		case MB_DEV_UART: 	return "MB_DEV_UART";
		case MB_DEV_QR: 	return "MB_DEV_QR";
		case MB_DEV_SDRAM: 	return "MB_DEV_SDRAM";
		case MB_DEV_ENC: 	return "MB_DEV_ENC";
		case MB_DEV_NP: 	return "MB_DEV_NP";
		case MB_DEV_REG: 	return "MB_DEV_REG";
		default: 			return "";
	}
}

typedef struct {
  int num;
  struct {
    uint32_t cod;
    uint32_t num;
  } dev[32];
} sFpgaCfg, *psFpgaCfg;

#endif
