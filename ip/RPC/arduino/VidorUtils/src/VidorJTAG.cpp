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
	#include "utility/jtag_host.h"
}

#include "VidorJTAG.h"

#define JI_PULSE_NCONFIG 0x001
#define JI_CHECK_STATUS  0x004
#define JI_USER0         0x00c
#define JI_USER1         0x00e

#define JBC_WRITE        0
#define JBC_READ         1

#define JTAG_VENDOR_ID   0x06E
#define VJTAG_ID         0x84

VidorJTAGClass::VidorJTAGClass() :
	_id(-1)
{
}

VidorJTAGClass::~VidorJTAGClass()
{
}

int VidorJTAGClass::begin(bool reset)
{
	LIBXSVF_HOST_SETUP();

	_libxsvfHost.tap_state = LIBXSVF_TAP_INIT;
	libxsvf_tap_walk(&_libxsvfHost, LIBXSVF_TAP_RESET);

	if (reset) {
		VidorJTAGClass::reset();
		delay(100);
	}

	if (!checkStatus()) {
		return 0;
	}

	_id = idScan();
	if (_id == -1) {
		return 0;
	}

	_lastVIR = -1;

	return 1;
}

void VidorJTAGClass::end()
{
	libxsvf_tap_walk(&_libxsvfHost, LIBXSVF_TAP_RESET);

	LIBXSVF_HOST_SHUTDOWN();
}

int VidorJTAGClass::reset()
{
	if (!loadInstruction(JI_PULSE_NCONFIG)) {
		return 0;
	}

	libxsvf_tap_walk(&_libxsvfHost, LIBXSVF_TAP_DRSHIFT);

	return 1;
}

int VidorJTAGClass::writeBuffer(uint32_t address, const uint32_t* data, size_t len)
{
	if (!loadVirtualInstruction(JBC_WRITE)) {
		return -1;
	}

	if (!loadInstruction(JI_USER0)) {
		return -1;
	}
	libxsvf_tap_walk(&_libxsvfHost, LIBXSVF_TAP_DRSHIFT);

	address = (address << 2) | 0x00000003;
	jtag_host_pulse_tdi((uint8_t*)&address, sizeof(address));
	jtag_host_pulse_tdi((uint8_t*)data, len * sizeof(data[0]));
	jtag_host_pulse_tdi_0(2);

	return len;
}

int VidorJTAGClass::readBuffer(uint32_t address, uint32_t* data, size_t len)
{
	if (!loadVirtualInstruction(JBC_WRITE)) {
		return -1;
	}

	if (!loadInstruction(JI_USER0)) {
		return -1;
	}
	libxsvf_tap_walk(&_libxsvfHost, LIBXSVF_TAP_DRSHIFT);

	address = (address << 2) | 0x00000003;
	jtag_host_pulse_tdi((uint8_t*)&address, sizeof(address));

	if (!loadVirtualInstruction(JBC_READ)) {
		return -1;
	}

	if (!loadInstruction(JI_USER0)) {
		return -1;
	}
	libxsvf_tap_walk(&_libxsvfHost, LIBXSVF_TAP_DRSHIFT);

	jtag_host_pulse_tdo((uint8_t*)data,  sizeof(data[0]));
	jtag_host_pulse_tdo((uint8_t*)data, len * sizeof(data[0]));
	return len;
}

int VidorJTAGClass::checkStatus()
{
	if (!loadInstruction(JI_CHECK_STATUS)) {
		return 0;
	}
	libxsvf_tap_walk(&_libxsvfHost, LIBXSVF_TAP_DRSHIFT);

	int tdo;

	for (int i = 0; i < 409; i++) {
		tdo = jtag_host_pulse_tdo_bit();
	}

	libxsvf_tap_walk(&_libxsvfHost, LIBXSVF_TAP_DRUPDATE);
	libxsvf_tap_walk(&_libxsvfHost, LIBXSVF_TAP_IDLE);

	return (tdo != 0);
}

int VidorJTAGClass::idScan()
{
	loadInstruction(JI_USER1);
	libxsvf_tap_walk(&_libxsvfHost, LIBXSVF_TAP_DRSHIFT);
	jtag_host_pulse_tdi_0(64);
	libxsvf_tap_walk(&_libxsvfHost, LIBXSVF_TAP_DRUPDATE);
	loadInstruction(JI_USER0);

	uint32_t record = 0;

	for (int i = 0; i < 8; i++) {
		libxsvf_tap_walk(&_libxsvfHost, LIBXSVF_TAP_DRSHIFT);

		record = (record >> 4) | (jtag_host_pulse_tdio(4, 0x00) << 28);

		libxsvf_tap_walk(&_libxsvfHost, LIBXSVF_TAP_DRUPDATE);
		libxsvf_tap_walk(&_libxsvfHost, LIBXSVF_TAP_IDLE);
	}

	if (((record >> 8) & 0x7ff) != JTAG_VENDOR_ID) {
		return -1;
	}

	_numSlaves = (record >> 19) & 0xff;
	for (_numSlaveBits = 0; (1 << _numSlaveBits) < (_numSlaves + 1); _numSlaveBits++);
	_virSize = record & 0xff;

	for (int j = 0; j < _numSlaves; j++) {
		record = 0;

		for (int i = 0; i < 8; i++) {
			libxsvf_tap_walk(&_libxsvfHost, LIBXSVF_TAP_DRSHIFT);

			record = (record >> 4) | (jtag_host_pulse_tdio(4, 0x00) << 28);

			libxsvf_tap_walk(&_libxsvfHost, LIBXSVF_TAP_DRUPDATE);
			libxsvf_tap_walk(&_libxsvfHost, LIBXSVF_TAP_IDLE);
		}

		if (((record >> 19) & 0xff) == VJTAG_ID && ((record >> 8) & 0x7ff) == JTAG_VENDOR_ID) {
			return j;
		}
	}

	return -1;
}

int VidorJTAGClass::loadVirtualInstruction(uint32_t instruction)
{
	if (_lastVIR == instruction) {
		return 1;
	}

	if (!loadInstruction(JI_USER1)) {
		return 0;
	}

	libxsvf_tap_walk(&_libxsvfHost, LIBXSVF_TAP_DRSHIFT);

	uint32_t code = ((_id + 1) << _virSize) | instruction;

	jtag_host_pulse_tdi_instruction(_virSize + _numSlaveBits, code);

	libxsvf_tap_walk(&_libxsvfHost, LIBXSVF_TAP_DRUPDATE);

	_lastVIR = instruction;

	return 1;
}

int VidorJTAGClass::loadInstruction(uint32_t instruction)
{
	if (_libxsvfHost.tap_state != LIBXSVF_TAP_IDLE || _libxsvfHost.tap_state != LIBXSVF_TAP_RESET) {
		libxsvf_tap_walk(&_libxsvfHost, LIBXSVF_TAP_IDLE);
	}
	libxsvf_tap_walk(&_libxsvfHost, LIBXSVF_TAP_IRSHIFT);

	if (jtag_host_pulse_tdio_instruction(10, instruction) != 0x155) {
		return 0;
	}

	libxsvf_tap_walk(&_libxsvfHost, LIBXSVF_TAP_IRUPDATE);

	return 1;
}

VidorJTAGClass VidorJTAG;
