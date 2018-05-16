/**
 *
 */

#include <stdio.h>
#include <string.h>
#include <alt_types.h>
#include <sys/alt_stdio.h>
#include <system.h>

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
	//alt_u32 ret;

	//ret =
	irqPinSet(0, cmdRx);

	platformSetup();
	//aesTest();
	//i2cTest();
	//gpioTest();
	//sfTest();

	while (1) {
		//platformCmd();
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
