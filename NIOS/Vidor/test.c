/*
 * test.c
 *
 *  Created on: May 15, 2018
 *      Author: max
 */

//#define TEST

#ifdef TEST

#include <system.h>
#include "mb.h"

//#define I2C_0_TEST
#define I2C_1_TEST
//#define GPIO_TEST
//#define SF_TEST

#ifdef I2C_0_TEST
void i2c0Test(void)
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
#endif  //I2C_0_TEST

#ifdef I2C_1_TEST
void i2c1Test(void)
{
	int index = 1;
	alt_u32 volatile *rpc = (alt_u32*)DPRAM_BASE;

	// enable I2C index 1 - HDMI_I2C_BASE
	rpc[0] = MB_DEV_I2C | ((index & 0x0F)<<20) | 0x01;
	platformCmd();

	// write
	rpc[1] = 0x00;  //address
	rpc[2] = 0x01;  //len
	rpc[3] = 0x00000000;
	rpc[0] = MB_DEV_I2C | ((index & 0x0F)<<20) | 0x08;
	platformCmd();

	// read
	rpc[1] = 0x00;  //address
	rpc[2] = 0x80;  //len
	rpc[0] = MB_DEV_I2C | ((index & 0x0F)<<20) | 0x05;
	platformCmd();
}
#endif  //I2C_1_TEST


#ifdef GPIO_TEST
void gpioTest(void)
{
	alt_u32 volatile *rpc = (alt_u32*)DPRAM_BASE;
	alt_u32 pin;
	alt_u32 mode;
#if 0
	alt_u32 val;

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
#endif  // GPIO_TEST


#ifdef SF_TEST
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

}
#endif  //SF_TEST


#ifdef AES_TEST


alt_u32 aesTest(void)
{
	/*
	alt_u32 key[8] = {0x12345678, 0x12345678, 0x12345678, 0x12345678,
			          0x12345678, 0x12345678, 0x12345678, 0x12345678};
	alt_u32 dec[4] = {0x12345678, 0x12345678, 0x12345678, 0x12345678};
	alt_u32 enc[4] = {0, 0, 0, 0};
*/
#if 0
	alt_u8 key[32] = {"1234567890123456790123465798012"};
	alt_u8 plain[16] = {"12345678abcdfegh"};
#endif
#if 0
	alt_u8 key[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	alt_u8 plain[16] = {0x01, 0x47, 0x30, 0xf8, 0x0a, 0xc6, 0x25, 0xfe,
			            0x84, 0xf0, 0x26, 0xc6, 0x0b, 0xfd, 0x54, 0x7d};
#endif


#if 1
/*
	KEY = 8d2e60365f17c7df1040d7501b4a7b5a
	PLAINTEXT = 59b5088e6dadc3ad5f27a460872d5929
	CIPHERTEXT = a02600ecb8ea77625bba6641ed5f5920
*/
	alt_u8 key[32] = {0x8d, 0x2e, 0x60, 0x36, 0x5f, 0x17, 0xc7, 0xdf,
			          0x10, 0x40, 0xd7, 0x50, 0x1b, 0x4a, 0x7b, 0x5a};
	alt_u8 plain[16] = {0x59, 0xb5, 0x08, 0x8e, 0x6d, 0xad, 0xc3, 0xad,
			            0x5f, 0x27, 0xa4, 0x60, 0x87, 0x2d, 0x59, 0x29};
#endif
/*
	KEY = 2d0860dae7fdb0bd4bfab111f615227a
	PLAINTEXT = a02600ecb8ea77625bba6641ed5f5920
	CIPHERTEXT = 5241ead9a89ca31a7147f53a5bf6d96a
*/
/*
	KEY = 7f498a034f6113a73abd442bade3fb10
	PLAINTEXT = 5241ead9a89ca31a7147f53a5bf6d96a
	CIPHERTEXT = 22f09171bc67d0661d1c25f181a69f33
*/

	alt_u8 enc[16];
	alt_u8 new[16];

	aesKeyWr((alt_u32*)key);
	aesDataWr((alt_u32*)plain);
	aesDataRd((alt_u32*)enc);
	aesConfig(AES_CFG_KEY_128, AES_CFG_ENCODE);

	aesKeyWr((alt_u32*)key);
	aesDataWr((alt_u32*)enc);
	aesDataRd((alt_u32*)new);
	aesConfig(AES_CFG_KEY_128, AES_CFG_DECODE);
	return 0;
}

#endif // AES_TEST

#endif


