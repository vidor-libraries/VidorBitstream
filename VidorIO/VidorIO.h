#include "VidorUtils.h"
#include "defines.h"

#ifndef __VIDOR_IO_H__
#define __VIDOR_IO_H__

class VidorIO {

public:
	static void pinMode(uint32_t pin, uint32_t mode) {
		uint32_t rpc[256];
		rpc[0] = MB_DEV_GPIO | 0x01;
		rpc[1] = pin;

		switch (mode) {
			case OUTPUT:
				rpc[2] = 2;
				break;
			case INPUT:
				rpc[2] = 1;
				break;
			default:
				rpc[2] = mode;
		}
		mbCmdSend(rpc, 3);
	}

	static void digitalWrite(uint32_t pin, uint32_t mode) {
		uint32_t rpc[256];
		rpc[0] = MB_DEV_GPIO | 0x02;
		rpc[1] = pin;
		rpc[2] = mode;
		mbCmdSend(rpc, 3);
	}

	static int digitalRead(uint32_t pin) {
		uint32_t rpc[256];
		rpc[0] = MB_DEV_GPIO | 0x03;
		rpc[1] = pin;
		return mbCmdSend(rpc, 2);
	}

	static int period;

	static void analogWriteResolution(int bits, int frequency) {

		uint32_t rpc[256];
		period = 2 << bits;
		int prescaler = (2 * F_CPU / frequency) / period;

		rpc[0] = MB_DEV_GPIO | 0x04;
		rpc[1] = prescaler;    // prescaler
		rpc[2] =  period;    // period
		mbCmdSend(rpc, 3);
	}

	static void analogWrite(uint32_t pin, uint32_t mode) {

		uint32_t rpc[256];

		if (period == -1) {
			// sane default
			analogWriteResolution(8, 490);
		}

		rpc[0] = MB_DEV_GPIO | 0x05;
		rpc[1] = pin;
		rpc[2] = mode;
		rpc[3] = period - mode;
		mbCmdSend(rpc, 4);
	}

	/* I2C functions */

	static int enableI2C(int index,int sda,int scl,int mode) {

		if (sda == -1)
			goto set_scl;

		if(sda<=14){
			sda=sda-0 ;
			pinMode(sda,INPUT);
			pinMode(sda+140,mode);
		} else {
			sda=sda-A0;
			pinMode(sda,INPUT);
			pinMode(sda+133,mode);
		}

set_scl:
		if (scl == -1)
			goto i2c_apply;

		if(scl<=14){
			scl=scl-0 ;
			pinMode(scl,INPUT);
			pinMode(scl+140,mode);
		} else {
			scl=scl-A0;
			pinMode(scl,INPUT);
			pinMode(scl+133,mode);
		}

i2c_apply:
		uint32_t rpc[1];
		rpc[0] = MB_DEV_I2C | ((index & 0x0F)<<20) | 0x01;
		return mbCmdSend(rpc, 1);
	}

	static void setI2CClock(int index, int baud) {
		uint32_t rpc[2];
		rpc[0] = MB_DEV_I2C | ((index & 0x0F)<<20) | 0x02;
		rpc[1] = baud;
		mbCmdSend(rpc, 2);
	}

	static void disableI2C(int index) {
		uint32_t rpc[1];
		rpc[0] = MB_DEV_I2C | ((index & 0x0F)<<20) | 0x03;
		mbCmdSend(rpc, 1);
	}

	/* UART functions */
	static void enableUART(int tx,int rx) {
		if(tx<=14){
			tx=tx-0 ;
			pinMode(tx,INPUT);
			pinMode(tx+140,4);
		} else {
			tx=tx-A0;
			pinMode(tx,INPUT);
			pinMode(tx+133,4);
		}
		if(rx<=14){
			rx=rx-0 ;
			pinMode(rx+140,INPUT);
			pinMode(rx,INPUT);
		} else {
			rx=rx-A0;
			pinMode(rx+133,INPUT);
			pinMode(rx,INPUT);
		}
	}
	static void setUART(int index, int baud, int config) {}
	static void disableUART(int index) {}

	static uint8_t readUART(int index) {
		uint32_t rpc[256];
		rpc[0] = MB_DEV_UART | ((index & 0x0F)<<20) | 0x04;
		return mbCmdSend(rpc, 1);
	}

	static uint8_t readUART(int index, uint8_t* buf, size_t howMany) {
		uint32_t rpc[256];
		rpc[0] = MB_DEV_UART | ((index & 0x0F)<<20) | 0x05;
		rpc[1] = howMany;
		mbCmdSend(rpc, 2);
		size_t i;
		for( i=0; i<1+(howMany+3)/4; i++) {
			jtagReadBuffer(MB_BASE+2+i, (uint8_t*)&rpc[2+i], 1);
		}
		memcpy(buf, &rpc[2], howMany);
		return 0;
	}

	static uint8_t availableUART(int index) {
		uint32_t rpc[256];
		rpc[0] = MB_DEV_UART | ((index & 0x0F)<<20) | 0x06;
		return mbCmdSend(rpc, 1);
	}

	static void writeUART(int index, uint8_t data) {
		uint32_t rpc[256];
		rpc[0] = MB_DEV_UART | ((index & 0x0F)<<20) | 0x07;
		rpc[1] = data;
		mbCmdSend(rpc, 2);
	}

	static void writeUART(int index, uint8_t* buf, size_t howMany) {
		uint32_t rpc[256];
		rpc[0] = MB_DEV_UART | ((index & 0x0F)<<20) | 0x08;
		rpc[1] = howMany;
		rpc[2] = (uint32_t)buf;
		mbCmdSend(rpc, 2+(rpc[1]+3)/4);
	}

	static void flushUART(int index) {}

	/* SPI functions */
	static void enableSPI(int index, int mosi, int miso, int sck, int cs) {
		if (mosi == -1)
			goto set_miso;
		if (mosi <= 14) {
			mosi = mosi - 0;
			pinMode(mosi, INPUT);
			pinMode(mosi + 140, 5);
		} else {
			mosi = mosi - A0;
			pinMode(mosi, INPUT);
			pinMode(mosi + 133, 5);
		}
set_miso:
		if (miso == -1)
			goto set_sck;
		if (miso <= 14) {
			miso = miso - 0 ;
			pinMode(miso + 140, INPUT);
			pinMode(miso, INPUT);
		} else {
			miso = miso - A0;
			pinMode(miso + 133, INPUT);
			pinMode(miso, INPUT);
		}
set_sck:
		if (sck == -1)
			goto set_cs;
		if (sck <= 14) {
			sck = sck - 0 ;
			pinMode(sck, INPUT);
			pinMode(sck + 140, 5);
		} else {
			sck = sck - A0;
			pinMode(sck, INPUT);
			pinMode(sck + 133, 5);
		}
set_cs:
		if (cs == -1)
			goto enable_spi;
		if (cs <= 14) {
			cs = cs - 0;
			pinMode(cs, INPUT);
			pinMode(cs + 140, 5);
		} else {
			cs = cs - A0;
			pinMode(cs, INPUT);
			pinMode(cs + 133, 5);
		}

enable_spi:
		uint32_t rpc[1];
		rpc[0] = MB_DEV_SPI | ((index & 0x0F)<<20) | 0x01;
		rpc[1] = index;
		mbCmdSend(rpc, 2);
	}

	static void setSPIMode(int index, int baud, int mode, int bitOrder) {
		uint32_t rpc[4];
		rpc[0] = MB_DEV_SPI | ((index & 0x0F)<<20) | 0x02;
		rpc[1] = baud * 2;
		rpc[2] = 0;
		rpc[3] = 0;
		mbCmdSend(rpc, 4);
	}

	static void disableSPI(int index) {}

	static uint8_t transferDataSPI(int index, uint8_t* buf, size_t howMany) {

		uint32_t rpc[256];
		if(howMany>128){
			return 0;
		}
		rpc[0] = MB_DEV_SPI | ((index & 0x0F)<<20) | 0x04;
		rpc[1] = howMany;
		memcpy(&rpc[2], buf, howMany);
		int ret = mbCmdSend(rpc, 2+(rpc[1]+3)/4);

		jtagReadBuffer(MB_BASE+2, (uint8_t*)&rpc[2], (howMany+3)/4);
		memcpy(buf, &rpc[2], howMany);

		return 0;
	}

	static uint8_t transferDataSPI(int index, uint8_t data) {
		/*
		uint32_t rpc[256];
		rpc[0] = MB_DEV_SPI | ((index & 0x0F)<<20) | 0x05;
		rpc[1] = data;
		uint8_t ret = mbCmdSend(rpc, 2);
		return ret;
		*/
		transferDataSPI(index, &data, 1);
		return data;
	}
};

#endif
