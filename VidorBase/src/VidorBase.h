#ifndef __VIDOR_LIB__
#define __VIDOR_LIB__

#define THIS_BITSTREAM_CHECKSUM 		"45c83d4fb44010509dc529cd8a8633e7"

#if defined(FPGA_BITSTREAM_CHECKSUM)
#error "Cannot load more than one bitstream"
#endif

#if !defined(FPGA_BITSTREAM_CHECKSUM)
#define FPGA_BITSTREAM_CHECKSUM 	THIS_BITSTREAM_CHECKSUM
#endif

#include "VidorUtils.h"

typedef enum {
	BASIC = 1,			// basic contains gpio functionlities
	ADVANCED_PWM,
	ADVANCED_I2C,
	ADVANCED_SPI,
	ADVANCED_UART,
	MIPI,
	VIDEO_OUT,
	MEMORY_MAP
} vidor_ips;

typedef enum {
	IRQ_WIRE = 1,
	IRQ_UART,
} vidor_irq;

#define BASE_ADDRESS			0

#define GPIO_BASE_ADDRESS		0x1000
#define GPIO_DIRECTION_OFFSET	0
#define GPIO_VALUE_OFFSET		4
#define GPIO_READ_OFFSET		8

#define IRQ_BASE_ADDRESS		0x4

#define GPIO_NUM_OFFSET			100

#define MEMORY_BASE_ADDRESS		0x10000

#define IRQ_PIN					33

#define MB_BASE     	0x00000000
#define MB_DEV_SF 		0x01000000
#define MB_DEV_GPIO 	0x02000000
#define MB_DEV_GFX  	0x03000000
#define MB_DEV_I2C  	0x04000000
#define MB_DEV_SPI  	0x05000000
#define MB_DEV_UART 	0x06000000
#define MB_DEV_QR   	0x07000000
#define MB_DEV_SDRAM 	0x08000000

typedef struct {
  int num;
  struct {
    uint32_t cod;
    uint32_t num;
  } dev[32];
} sFpgaCfg, *psFpgaCfg;

class VidorBase : public VidorUtils {
public:

	VidorBase() {}

	static void onInterrupt();

	int begin(bool jumpToApp = true) {
		int ret = VidorUtils::begin(jumpToApp);

		capabilities = 0xFF; //read(BASE_ADDRESS);
		attachInterrupt(IRQ_PIN, VidorBase::onInterrupt, FALLING);

		return ret;
	}

	int isCapableOf(vidor_ips ip) {
		return (capabilities | (1 << ip));
	}

	bool ready() {
		return capabilities != 0;
	}

	int writeMemory(int offset, uint8_t* buffer, size_t len) {
		if (isCapableOf(MEMORY_MAP)) {
			return write(MEMORY_BASE_ADDRESS + offset, buffer, len);
		}
		return -1;
	}

	int readMemory(int offset, uint8_t* buffer, size_t len) {
		if (isCapableOf(MEMORY_MAP)) {
			return read(MEMORY_BASE_ADDRESS + offset, buffer, len);
		}
		return -1;
	}

	void pinMode(uint32_t pin, uint32_t mode) {
		if (isCapableOf(BASIC)) {
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
	}

	void digitalWrite(uint32_t pin, uint32_t mode) {
		uint32_t rpc[256];
		rpc[0] = MB_DEV_GPIO | 0x02;
		rpc[1] = pin;
		rpc[2] = mode;
		mbCmdSend(rpc, 3);
	}

	int digitalRead(uint32_t pin) {
		uint32_t rpc[256];
		rpc[0] = MB_DEV_GPIO | 0x03;
		rpc[1] = pin;
		return mbCmdSend(rpc, 2);
	}

	void analogWrite(uint32_t pin, uint32_t mode) {
		uint32_t rpc[256];
		rpc[0] = MB_DEV_GPIO | 0x04;
		rpc[1] = pin;
		rpc[2] = mode;
		mbCmdSend(rpc, 3);
	}

	/* I2C functions */

	int enableI2C(int index) {
		uint32_t rpc[1];
		rpc[0] = MB_DEV_I2C | ((index & 0x0F)<<20) | 0x01;
		return mbCmdSend(rpc, 1);
	}

	void setI2CClock(int index, int baud) {
		uint32_t rpc[2];
		rpc[0] = MB_DEV_I2C | ((index & 0x0F)<<20) | 0x02;
		rpc[1] = baud;
		mbCmdSend(rpc, 2);
	}

	void disableI2C(int index) {
		uint32_t rpc[1];
		rpc[0] = MB_DEV_I2C | ((index & 0x0F)<<20) | 0x03;
		mbCmdSend(rpc, 1);
	}

	/* UART functions */
	void enableUART(int tx,int rx) {
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
	void setUART(int index, int baud, int config) {}
	void disableUART(int index) {}

	uint8_t readUART(int index) {
		uint32_t rpc[256];
		rpc[0] = MB_DEV_UART | ((index & 0x0F)<<20) | 0x04;
		return mbCmdSend(rpc, 1);
	}

	uint8_t readUART(int index, uint8_t* buf, size_t howMany) {
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

	uint8_t availableUART(int index) {
		uint32_t rpc[256];
		rpc[0] = MB_DEV_UART | ((index & 0x0F)<<20) | 0x06;
		return mbCmdSend(rpc, 1);
	}

	void writeUART(int index, uint8_t data) {
		uint32_t rpc[256];
		rpc[0] = MB_DEV_UART | ((index & 0x0F)<<20) | 0x07;
		rpc[1] = data;
		mbCmdSend(rpc, 2);
	}

	void writeUART(int index, uint8_t* buf, size_t howMany) {
		uint32_t rpc[256];
		rpc[0] = MB_DEV_UART | ((index & 0x0F)<<20) | 0x08;
		rpc[1] = howMany;
		rpc[2] = (uint32_t)buf;
		mbCmdSend(rpc, 2+(rpc[1]+3)/4);
	}

	void flushUART(int index) {}

	/* SPI functions */
	void enableSPI(int index) {}
	void setSPIMode(int index, int baud, int mode, int bitOrder) {}
	void disableSPI(int index) {}

	uint8_t transferDataSPI(int index, uint8_t* buf, size_t howMany) {
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

	uint8_t transferDataSPI(int index, uint8_t data) {
		uint32_t rpc[256];
		rpc[0] = MB_DEV_SPI | ((index & 0x0F)<<20) | 0x02;
		rpc[1] = data;
		return mbCmdSend(rpc, 2);
	}

private:

	uint32_t capabilities = 0;
};

extern VidorBase VD;

#endif
