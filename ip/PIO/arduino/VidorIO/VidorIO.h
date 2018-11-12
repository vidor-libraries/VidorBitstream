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
#include "VidorIP.h"
#include "defines.h"

#ifndef __VIDOR_IO_H__
#define __VIDOR_IO_H__

#define VIDOR_PWM 	0x55

class VidorIO : public VidorIP {

public:

	VidorIO(int _base) : base(_base) {
	}

	void pinMode(uint32_t pin, uint32_t mode) {
		uint32_t rpc[4];

		// get giid and chan for required pin (pin here has the complete numbering scheme)
		// TODO: should I ask this to UID 0 == FPGA ?
		int ret = init(PIO_UID, pin + 32*base);
		if (ret < 0) {
			// ignore and go ahead
		}

		rpc[0] = RPC_CMD(info.giid, pin, 5);

		switch (mode) {
			case OUTPUT:
			case INPUT:
				rpc[1] = 0;
				break;
			case VIDOR_PWM:
				rpc[1] = 1;
				break;
			default:
				rpc[1] = mode;
				break;
		}

		switch (mode) {
			case OUTPUT:
				rpc[2] = 1;
				break;
			case INPUT:
				rpc[2] = 0;
				break;
			default:
				rpc[2] = mode;
				break;
		}
		rpc[3] = 0;
		VidorMailbox.sendCommand(rpc, 4);
	}

	void digitalWrite(uint32_t pin, uint32_t mode) {
		uint32_t rpc[2];
		rpc[0] = RPC_CMD(info.giid, pin, 6);
		rpc[1] = mode;
		VidorMailbox.sendCommand(rpc, 2);
	}

	int digitalRead(uint32_t pin) {
		uint32_t rpc[1];
		rpc[0] = RPC_CMD(info.giid, pin, 7);
		return VidorMailbox.sendCommand(rpc, 1);
	}

	int begin() {
		return 0;
	};

  private:
	int base;
	bool initialized = false;
};

class VidorPWM : public VidorIP {

public:

	VidorPWM(int _base) : base(_base) {}

	void analogWriteResolution(int bits, int frequency) {
		uint32_t rpc[3];
		period = 2 << bits;
		int prescaler = (2 * F_CPU / frequency) / period;

		rpc[0] = RPC_CMD(info.giid, info.chn, 5);
		rpc[1] = prescaler;
		rpc[2] = period;
		VidorMailbox.sendCommand(rpc, 3);
	}

	void analogWrite(uint32_t pin, uint32_t mode) {

		uint32_t rpc[3];

		if (period == -1) {
			// sane default
			analogWriteResolution(8, 490);
		}

		// Always call init() without checking the result
		init(PWM_UID, base * 32 + pin);

		rpc[0] = RPC_CMD(info.giid, pin, 6);
		rpc[1] = mode;
		rpc[2] = period - mode;
		VidorMailbox.sendCommand(rpc, 3);
	}

	int begin() {
	};

  private:
	int base;
	int giid;
	int period = -1;
};

static VidorPWM* pwm_instance[2] = {NULL, NULL};

class VidorIOContainer {
	public:

		VidorIO* io_instance[3] = {NULL, NULL, NULL};

		void pinMode(int pin, int mode) {
			pin = digital_to_fpga(pin);
			if (io_instance[pin >> 5] == NULL) {
				io_instance[pin >> 5] = new VidorIO(pin >> 5);
			}
			io_instance[pin >> 5]->pinMode(pin % 32, mode);
		}

		void digitalWrite(int pin, int value) {
			pin = digital_to_fpga(pin);
			int base = pin >> 5;
			if (io_instance[base] == NULL) {
				return;
			}
			io_instance[base]->digitalWrite(pin % 32, value);
		}

		int digitalRead(int pin) {
			pin = digital_to_fpga(pin);
			int base = pin >> 5;
			if (io_instance[base] == NULL) {
				return -1;
			}
			return io_instance[base]->digitalRead(pin % 32);

		}
		void analogWriteResolution(int bits, int frequency) {
			if (pwm_instance[0] == NULL) {
				pwm_instance[0] = new VidorPWM(0);
			}
			pwm_instance[0]->analogWriteResolution(bits, frequency);
		}

		void analogWrite(int pin, int mode) {
			pin = digital_to_fpga(pin);
			if (pwm_instance[pin/32] == NULL) {
				pwm_instance[pin/32] = new VidorPWM(pin/32);
			}
			pwm_instance[pin/32]->analogWrite(pin % 32, mode);
		}
};

#endif
