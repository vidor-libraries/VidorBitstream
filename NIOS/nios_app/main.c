/**
 *
 */
#include <string.h>

#include "platform.h"
#include "gfx.h"
#include "gpio.h"

extern alt_u32 _alt_partition_text_ram_start  __attribute__((section(".data")));
extern alt_u32 _alt_partition_text_ram_end  __attribute__((section(".data")));

void app_main(void);
void cmdRx(alt_u32 cmd);

/**
 *
 */
//void app_main(void)
int main(void)
{
	// logo iniziale
	gfxInit(0);

	platformSetup();
	//aesTest();
	//i2cTest();
	//gpioTest();
	//sfTest();
	//i2c1Test();
	//uartTest();
	//aesTest();

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
