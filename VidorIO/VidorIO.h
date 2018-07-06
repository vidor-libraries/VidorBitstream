#include "VidorUtils.h"
#include "defines.h"

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

	static void analogWrite(uint32_t pin, uint32_t mode) {
		uint32_t rpc[256];
		rpc[0] = MB_DEV_GPIO | 0x04;
		rpc[1] = pin;
		rpc[2] = mode;
		mbCmdSend(rpc, 3);
	}

	/* I2C functions */

	static int enableI2C(int index) {
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
	static void enableSPI(int index) {
		//pinMode(SCK_BB, 4);
		//pinMode(MOSI_BB, 4);
		//pinMode(MISO_BB, 4);
	}

	static void setSPIMode(int index, int baud, int mode, int bitOrder) {}
	static void disableSPI(int index) {}

	static uint8_t transferDataSPI(int index, uint8_t* buf, size_t howMany) {
		uint32_t rpc[256];
		if(howMany>128){
			return 0;
		}
		rpc[0] = MB_DEV_SPI | ((index & 0x0F)<<20) | 0x01;
		rpc[1] = howMany;
		memcpy(&rpc[2], buf, howMany);
		mbCmdSend(rpc, 2+(rpc[1]+3)/4);

		size_t i;
		for(i=0; i<1+(howMany+3)/4; i++){
			jtagReadBuffer(MB_BASE+2+i, (uint8_t*)&rpc[2+i], 1);
		}
		memcpy(buf, &rpc[2], howMany);

		return 0;
	}

	static uint8_t transferDataSPI(int index, uint8_t data) {
		uint32_t rpc[256];
		rpc[0] = MB_DEV_SPI | ((index & 0x0F)<<20) | 0x02;
		rpc[1] = data;
		return mbCmdSend(rpc, 2);
	}
}
