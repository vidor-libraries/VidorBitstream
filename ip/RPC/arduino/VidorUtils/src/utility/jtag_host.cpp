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

extern "C" {
	#include "jtag_host.h"
}

#include <Arduino.h>
#include <SPI.h>

#define TMS     28 // PA14             | SERCOM2/ PAD[2]
#define TCK     27 // PA13 -> SPI CLK  | SERCOM2/ PAD[1]
#define TDO     29 // PA15 -> MISO     | SERCOM2/ PAD[3]
#define TDI     26 // PA12 -> MOSI     | SERCOM2/ PAD[0]

#define TMS_HIGH() (digitalPinToPort(TMS)->OUTSET.reg = digitalPinToBitMask(TMS))
#define TMS_LOW()  (digitalPinToPort(TMS)->OUTCLR.reg = digitalPinToBitMask(TMS))

#define TCK_HIGH() (digitalPinToPort(TCK)->OUTSET.reg = digitalPinToBitMask(TCK))
#define TCK_LOW()  (digitalPinToPort(TCK)->OUTCLR.reg = digitalPinToBitMask(TCK))
#define TCK_PMUX() (digitalPinToPort(TCK)->PINCFG[g_APinDescription[TCK].ulPin].bit.PMUXEN = 1)
#define TCK_UNPMUX() (digitalPinToPort(TCK)->PINCFG[g_APinDescription[TCK].ulPin].bit.PMUXEN = 0)

#define TDI_HIGH() (digitalPinToPort(TDI)->OUTSET.reg = digitalPinToBitMask(TDI))
#define TDI_LOW()  (digitalPinToPort(TDI)->OUTCLR.reg = digitalPinToBitMask(TDI))
#define TDI_PMUX() (digitalPinToPort(TDI)->PINCFG[g_APinDescription[TDI].ulPin].bit.PMUXEN = 1)
#define TDI_UNPMUX() (digitalPinToPort(TDI)->PINCFG[g_APinDescription[TDI].ulPin].bit.PMUXEN = 0)

#define TDO_PMUX() (digitalPinToPort(TDO)->PINCFG[g_APinDescription[TDO].ulPin].bit.PMUXEN = 1)
#define TDO_UNPMUX() (digitalPinToPort(TDO)->PINCFG[g_APinDescription[TDO].ulPin].bit.PMUXEN = 0)

#define TMS_WRITE(val) {if (val) { TMS_HIGH(); } else { TMS_LOW(); }}
#define TDI_WRITE(val) {if (val) { TDI_HIGH(); } else { TDI_LOW(); }}
#define TDO_READ() ((digitalPinToPort(TDO)->IN.reg & digitalPinToBitMask(TDO)) != 0)

#define SPI_JTAG SPI1
SPISettings JTAG_SPISettings(12000000, LSBFIRST, SPI_MODE0);

extern "C" {
	int jtag_host_setup()
	{
		pinMode(TMS, OUTPUT);
		pinMode(TCK, OUTPUT);
		pinMode(TDO, INPUT);
		pinMode(TDI, OUTPUT);

		TMS_LOW();
		TCK_LOW();
		TDI_LOW();

		SPI_JTAG.begin();
		SPI_JTAG.beginTransaction(JTAG_SPISettings);

		TCK_UNPMUX();
		TDI_UNPMUX();

		return 0;
	}

	int jtag_host_shutdown()
	{
		TMS_LOW();
		TCK_LOW();
		TDI_LOW();

		pinMode(TMS, INPUT);
		pinMode(TCK, INPUT);
		pinMode(TDO, INPUT);
		pinMode(TDI, INPUT);

		return 0;
	}

	void jtag_host_pulse_tck(int tms)
	{
		TMS_WRITE(tms);

		TCK_LOW();
		TCK_HIGH();
	}

	int jtag_host_pulse_tdo_bit()
	{
		TDI_LOW();

		TCK_LOW();
		TCK_HIGH();
		
		return TDO_READ();
	}

	unsigned int jtag_host_pulse_tdio_instruction(int bits, unsigned int out)
	{
		unsigned int in = 0;

		for (int i = 0; i < bits; i++) {
			TDI_WRITE(out & 1);

			if (i != (bits - 1)) {
				TCK_LOW();
				TCK_HIGH();
			
				in |= (TDO_READ() << i);

				out >>= 1;
			}
		}

		return in;
	}

	void jtag_host_pulse_tdi_instruction(int bits, unsigned int out)
	{
		for (int i = 0; i < bits; i++) {
			TDI_WRITE(out & 1);

			if (i != (bits - 1)) {
				TCK_LOW();
				TCK_HIGH();
			
				out >>= 1;
			}
		}
	}

	unsigned int jtag_host_pulse_tdio(int bits, unsigned int out)
	{
		unsigned int in = 0;

		for (int i = 0; i < bits; i++) {
			TDI_WRITE(out & 1);

			TCK_LOW();
			TCK_HIGH();

			in |= (TDO_READ() << i);

			out >>= 1;
		}

		return in;
	}

	void jtag_host_pulse_tdi_0(int bits)
	{
		digitalWrite(TDI, LOW);

		for (int i = 0; i < bits; i++) {
			TCK_LOW();
			TCK_HIGH();
		}
	}

	void jtag_host_pulse_tdi(const uint8_t* data, size_t size)
	{
		TCK_LOW();

		TCK_PMUX();
		TDI_PMUX();

		for (size_t i = 0; i < size; i++) {
			SPI_JTAG.transfer(*data++);
		}

		TCK_UNPMUX();
		TDI_UNPMUX();
	}
	
	void jtag_host_pulse_tdo(uint8_t* data, size_t size)
	{
		TCK_LOW();

		TCK_PMUX();
		TDO_PMUX();

		for (size_t i = 0; i < size; i++) {
			*data = SPI_JTAG.transfer(0x00);
			data++;
		}

		TCK_UNPMUX();
		TDO_UNPMUX();
	}
}
