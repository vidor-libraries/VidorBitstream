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

#ifndef __VIDOR_FPGA_H__
#define __VIDOR_FPGA_H__

#define THIS_BITSTREAM_CHECKSUM 		"45c83d4fb44010509dc529cd8a8633e7"

#if defined(FPGA_BITSTREAM_CHECKSUM)
#error "Cannot load more than one bitstream"
#endif

#if !defined(FPGA_BITSTREAM_CHECKSUM)
#define FPGA_BITSTREAM_CHECKSUM 	THIS_BITSTREAM_CHECKSUM
#endif

#include "VidorUtils.h"
#include "defines.h"

#if FPGA_I2C_INTERFACES_COUNT > 0
#include "VidorI2C.h"
#endif

#if FPGA_SPI_INTERFACES_COUNT > 0
#include "VidorSPI.h"
#endif

#if FPGA_UART_INTERFACES_COUNT > 0
#include "VidorUART.h"
#endif

#if FPGA_ENCODERS_COUNT > 0
#include "VidorEncoder.h"
#endif

#if FPGA_NEOPIXEL_COUNT > 0
#include "Vidor_NeoPixel.h"
#endif

#if FPGA_CAMERA_COUNT > 0
#include "VidorCamera.h"
#endif

#if FPGA_GFX_COUNT > 0
#include "Vidor_GFX.h"
#endif

class VidorFPGA : public VidorUtils, public VidorIO {
public:

	VidorFPGA() {}

	static void onInterrupt();

	int begin(bool jumpToApp = true) {
		int ret = VidorUtils::begin(jumpToApp);

		attachInterrupt(IRQ_PIN, VidorFPGA::onInterrupt, FALLING);

		if (jumpToApp) {
			// wait one second to make sure jump was ok
			delay(1000);
			// reinitilize jtag chain
			VidorMailbox.begin();
		}

		return ret;
	}

	bool ready() {
		return (version() != 0) &&  (version() != 0xFFFFFFFF);
	}

	uint32_t version() {
		uint32_t rpc[1];
		uint32_t ver;

		rpc[0] = 0 | 1;
		ver = VidorMailbox.sendCommand(rpc, 1);
		return ver;
	}

	uint32_t printConfig(void)
	{
		uint32_t rpc[1];
		sFpgaCfg cfg;
		char str[64];

		rpc[0] = 0 | 2;
		VidorMailbox.sendCommand(rpc, 1);
		memset(&cfg, 0x00, sizeof(cfg));
		VidorMailbox.read(1, (uint32_t*)&cfg, (sizeof(cfg)+3)/4);

		int i;
		sprintf(str, "number of devices %u", cfg.num);
		Serial.println(str);
		for(i=0; i<cfg.num; i++){
			sprintf(str, "%u %08X %s", cfg.dev[i].num, cfg.dev[i].cod, configToString(cfg.dev[i].cod));
			Serial.println(str);
		}
		return 0;
	}

  uint8_t instance(void)
  {
    // TODO try to intance requestested IP
    return 0xFF;
  }
};

extern VidorFPGA FPGA;

#endif
