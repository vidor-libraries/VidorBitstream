#ifndef __VIDOR_UTILS_H__
#define __VIDOR_UTILS_H__

// Defines for fpga_bitstream_signature section

#include "jtag.h"

#define no_data		0xFF, 0xFF, 0xFF, 0xFF, \
					0xFF, 0xFF, 0xFF, 0xFF, \
					0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, \
					0xFF, 0xFF, 0xFF, 0xFF, \
					0x00, 0x00, 0x00, 0x00  \

#define NO_BOOTLOADER		no_data
#define NO_APP				no_data
#define NO_USER_DATA		no_data

__attribute__((weak)) void enableFpgaClock() {}
__attribute__((weak)) void disableFpgaClock() {}

class VidorUtils {
public:

	VidorUtils() {}

	static void onInterrupt();

	int begin(bool jumpToApp = true) {

		// Start clocking the FPGA; this function is declared weak and can be overridden
		// with a custom implementation (or can be left untouched if FPGA is clocked internally)
		enableFpgaClock();

		int ret = jtagInit();
		mbPinSet();

		if (ret == 0 && jumpToApp) {
			uint32_t ptr[1];
			ptr[0] = 0 | 3;
			mbEveSend(ptr, 1);
		}

		return (ret == 0);
	}

	void end() {
		disableFpgaClock();
		jtagDeinit();
	}

	void reload() {
		jtagReload();
	}

	int write(int address, const uint8_t* buffer, size_t len) {
		return jtagWriteBuffer(address, buffer, len/4);
	}

	int write(int address, uint32_t data) {
		return write(address, (uint8_t*)&data, 1);
	}

	int write(int address, int data) {
		return write(address, (uint8_t*)&data, 1);
	}

	int write(int address, uint8_t data) {
		uint32_t temp = (uint32_t)data;
		return write(address, (uint8_t*)&temp, 1);
	}

	int read(int address, uint8_t* buffer, size_t len) {
		return jtagReadBuffer(address, buffer, len);
	}

	int read(int address) {
		uint32_t temp;
		jtagReadBuffer(address, (uint8_t*)&temp, 1);
		return temp;
	}
};

#endif
