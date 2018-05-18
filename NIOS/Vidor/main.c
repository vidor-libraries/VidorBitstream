/**
 *
 */

#include "gpio.h"

void platformSetup(void);
void platformCmd(void);
void platformLoop(void);

void cmdRx(alt_u32 cmd);

/**
 *
 */
int main()
{
	platformSetup();
	//aesTest();
	//i2cTest();
	//gpioTest();
	//sfTest();
	//i2c1Test();
	irqPinSet(0, cmdRx);

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
