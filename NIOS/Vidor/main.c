/**
 *
 */

//#define BOOT_LOADER

#ifdef BOOT_LOADER
#include <alt_types.h>
#include "sf.h"

#define WORK_BASE (512*1024)
#define USER_BASE (1024*1024)
#endif /* BOOT_LOADER */

#include "gpio.h"

void platformSetup(void);
void platformCmd(void);
void platformLoop(void);

void cmdRx(alt_u32 cmd);

#include <system.h>
#include "aes.h"
#include "mb.h"

void signWrTest(void)
{
	uint32_t       JedecId;
	uint8_t        UniqueId[8];
	uint8_t        data[3*256];
	uint8_t        key[32];
	uint8_t        iv[16];
	struct AES_ctx ctx;
	uint32_t       ret;
	alt_u32 volatile *rpc = (alt_u32*)DPRAM_BASE;

	rpc[0] = MB_DEV_SF | 0x01;
	platformCmd();
	JedecId = rpc[1];

	rpc[0] = MB_DEV_SF | 0x02;
	platformCmd();
	memcpy((alt_u8*)&UniqueId, (alt_u8*)&rpc[2], 8);

	keygen(key, (uint8_t*)&JedecId, UniqueId);
	ivgen(iv, (uint8_t*)&JedecId, UniqueId);

	memcpy(data, "asdfredcvbhgfrtyk8964dfvghjf5d54", 32);

	AES_init_ctx(&ctx, key);
	AES_ctx_set_iv(&ctx, iv);
	AES_CBC_encrypt_buffer(&ctx, data, 3*256);

	rpc[0] = MB_DEV_SF | 0x06;
	rpc[1] = 0;
	platformCmd();

	rpc[0] = MB_DEV_SF | 0x07;
	rpc[1] = 0;
	rpc[2] = 0;
	rpc[3] = 256;
	memcpy((alt_u8*)&rpc[4], data, 256);
	platformCmd();

	rpc[0] = MB_DEV_SF | 0x06;
	rpc[1] = 1;
	platformCmd();

	rpc[0] = MB_DEV_SF | 0x07;
	rpc[1] = 1;
	rpc[2] = 0;
	rpc[3] = 256;
	memcpy((alt_u8*)&rpc[4], data+256, 256);
	platformCmd();

	rpc[0] = MB_DEV_SF | 0x06;
	rpc[1] = 2;
	platformCmd();

	rpc[0] = MB_DEV_SF | 0x07;
	rpc[1] = 2;
	rpc[2] = 0;
	rpc[3] = 256;
	memcpy((alt_u8*)&rpc[4], data+512, 256);
	platformCmd();

}




#if 0
/**
 * http://billauer.co.il/blog/2017/06/remote-update-intel-fpga-cyclone-iv/
 */
void do_remote_update(void) {
  alt_u32 app_bitstream_addr = 0x100000;
  alt_u32 mode = IORD_32DIRECT(REMOTE_UPDATE_0_BASE, 0) & 3;
  alt_u32 config_reason = IORD_32DIRECT(REMOTE_UPDATE_0_BASE, 0x64);

  if ((mode == 0) && (config_reason == 0)) {
    IOWR_32DIRECT(REMOTE_UPDATE_0_BASE, 0x30, 0); // Turn off watchdog
    IOWR_32DIRECT(REMOTE_UPDATE_0_BASE, 0x40, app_bitstream_addr);
    IOWR_32DIRECT(REMOTE_UPDATE_0_BASE, 0x74, 1); // Trigger reconfiguration
    while (1); // Wait briefly until configuration takes place
  }

}
#endif
/**
 *
 */
int main()
{
#ifdef BOOT_LOADER
	alt_u8 uid[8];
	alt_u8 sign[3*256];

	sfUniqueId(uid);

	sfSRRead(0, 0, sign, 256);
	sfSRRead(1, 0, sign+256, 256);
	sfSRRead(2, 0, sign+512, 256);

	//verifica firma
	//ret = signChk(uid, sign);

//come si verifica se c'è un sof valido?
// lo fa l'ip remote update da sola con la spunta su pof checking
//The POF checking feature detects and verifies the existence of
//an application configuration image before the image is loaded.
#endif /* BOOT_LOADER */

	// TODO verifica firma


	platformSetup();
	//aesTest();
	//i2cTest();
	//gpioTest();
	//sfTest();
	//i2c1Test();
	//uartTest();
	//aesTest();
#if 1
	int ret;

	ret = sign();
	if(ret){
		while(1);
	}
#endif


	irqPinSet(0, cmdRx);
	//intPinInit(1, 0);

	while (1) {
		platformCmd();
		platformLoop();
	};

	return 0;
}

/**
 *
 */
void cmdRx(alt_u32 arg)
{
	platformCmd();
}
