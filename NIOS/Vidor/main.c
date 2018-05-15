/**
 *
 */

#include <stdio.h>
#include <string.h>
#include <sys/alt_stdio.h>
#include <system.h>

#include "i2c.h"
#include "gpio.h"
#include "mb.h"
#include "sf.h"
#include "gfx.h"
#include "spi.h"
#include "uart.h"

void platformSetup(void);
void platformCmd(void);
void platformLoop(void);

/**
 * TODO togliere, solo per debug
 */

#if 0
void i2cTest(void)
{
	int index = 0;
	alt_u32 volatile *rpc = (alt_u32*)DPRAM_BASE;

	// enable I2C index 0 - CSI_I2C_BASE
	rpc[0] = MB_DEV_I2C | ((index & 0x0F)<<20) | 0x01;
	platformCmd();

	// start transmission
	rpc[1] = 0x36;
	rpc[2] = 0x00;
	rpc[0] = MB_DEV_I2C | ((index & 0x0F)<<20) | 0x04;
	platformCmd();

	// write
	rpc[1] = 0x01;
	rpc[0] = MB_DEV_I2C | ((index & 0x0F)<<20) | 0x08;
	platformCmd();

	rpc[1] = 0x03;
	rpc[0] = MB_DEV_I2C | ((index & 0x0F)<<20) | 0x08;
	platformCmd();

	rpc[1] = 0x01;
	rpc[0] = MB_DEV_I2C | ((index & 0x0F)<<20) | 0x08;
	platformCmd();

	// stop
	rpc[0] = MB_DEV_I2C | ((index & 0x0F)<<20) | 0x07;
	platformCmd();
}
#endif
void gpioTest(void)
{
	alt_u32 volatile *rpc = (alt_u32*)DPRAM_BASE;
	alt_u32 pin;
	alt_u32 mode;
	alt_u32 val;
#if 0
	/* set mode */
	pin  = 32;
	mode = 2;
	rpc[1] = pin;
	rpc[2] = mode;
	rpc[0] = MB_DEV_GPIO | 0x01;
	platformCmd();

	/* write */
	val = 0;
	rpc[1] = pin;
	rpc[2] = val;
	rpc[0] = MB_DEV_GPIO | 0x02;
	platformCmd();

	/* write */
	val = 1;
	rpc[1] = pin;
	rpc[2] = val;
	rpc[0] = MB_DEV_GPIO | 0x02;
	platformCmd();

	/* read */
	rpc[1] = pin;
	rpc[0] = MB_DEV_GPIO | 0x03;
	platformCmd();
#endif

	/* pwm */
	pin  = 32;
	mode = 3;
	rpc[1] = pin;
	rpc[2] = mode;
	rpc[0] = MB_DEV_GPIO | 0x01;
	platformCmd();

	rpc[1] = 1000;//prescaler
	rpc[2] = 120;//period
	rpc[0] = MB_DEV_GPIO | 0x04;
	platformCmd();

	rpc[1] = pin;
	rpc[2] = 60;
	rpc[3] = 40;
	rpc[0] = MB_DEV_GPIO | 0x05;
	platformCmd();

}

void sfTest(void)
{
	alt_u32 volatile *rpc = (alt_u32*)DPRAM_BASE;

	// JedecId
	rpc[0] = MB_DEV_SF | 0x01;
	platformCmd();

	// UniqueId
	memset(rpc, 0, 32);
	rpc[0] = MB_DEV_SF | 0x02;
	platformCmd();

	// read
	memset(rpc, 0, 32);
	rpc[1] = 0;
	rpc[2] = 16;
	rpc[0] = MB_DEV_SF | 0x05;
	platformCmd();

	// read
	memset(rpc, 0, 32);
	rpc[1] = 64*1024;
	rpc[2] = 16;
	rpc[0] = MB_DEV_SF | 0x05;
	platformCmd();

	// erase first 64K
	rpc[1] = 2;
	rpc[2] = 0;
	rpc[0] = MB_DEV_SF | 0x03;
	platformCmd();

	// erase second 64K
	rpc[1] = 2;
	rpc[2] = 1;
	rpc[0] = MB_DEV_SF | 0x03;
	platformCmd();



	// program
	rpc[1] = 0;
	rpc[2] = 16;
	strcpy(&rpc[3], "01234567890abcdef");
	rpc[0] = MB_DEV_SF | 0x04;
	platformCmd();

	// program
	rpc[1] = 64*1024;
	rpc[2] = 16;
	strcpy(&rpc[3], "fedcba9876543210");
	rpc[0] = MB_DEV_SF | 0x04;
	platformCmd();


	// read
	memset(rpc, 0, 32);
	rpc[1] = 0;
	rpc[2] = 32;
	rpc[0] = MB_DEV_SF | 0x05;
	platformCmd();

	// read
	memset(rpc, 0, 32);
	rpc[1] = 64*1024;
	rpc[2] = 32;
	rpc[0] = MB_DEV_SF | 0x05;
	platformCmd();


}
/**
 *
 */
int main()
{
	platformSetup();
	//int ret;

	// update serial flash with FPGA binary
	//ret = sfWrite(0x00800000, 181344, SF_WRFLG_ERASE | SF_WRFLG_PROGRAM | SF_WRFLG_VERIFY);
	//ret = sfWrite(0x00800000, 181344, SF_WRFLG_VERIFY);
/*
	alt_u32 sf_id;
	alt_u8  sf_sn[8];

	sf_id = sfDetect();
	sfReadId(sf_sn);

	// Security Register test
	alt_u8 data[256];

	sfSRRead(0, 0, data, 256);
	strcpy(data, "111-Security Register write test-111");
	sfSRProgram(0, 0, data, 256);

	sfSRRead(1, 0, data, 256);
	strcpy(data, "222-Security Register write test-222");
	sfSRProgram(1, 0, data, 256);

	sfSRRead(2, 0, data, 256);
	strcpy(data, "333-Security Register write test-333");
	sfSRProgram(2, 0, data, 256);

	sfSRRead(0, 0, data, 256);
	sfSRRead(1, 0, data, 256);
	sfSRRead(2, 0, data, 256);
*/
/*
	sfSRErase(alt_u8 reg);

	sfSRLock(alt_u8 reg);
*/
	//aesTest();
	//i2cTest();
	//gpioTest();
	sfTest();

// TODO	mbInit(rxCmd);

	/* Event loop never exits. */
	while (1) {
		platformCmd();
		platformLoop();
	};

	return 0;
}
