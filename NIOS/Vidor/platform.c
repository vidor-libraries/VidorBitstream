/*
 * platform.c
 *
 *  Created on: May 9, 2018
 *      Author: max
 */
#include <stdio.h>
#include <system.h>

#include "i2c.h"
#include "gpio.h"
#include "mb.h"
#include "sf.h"
#include "gfx.h"
#include "spi.h"
#include "uart.h"
#include "qr.h"

/**
 */
typedef struct {
	void(*setup)(int);
	void(*cmd)(void);
	void(*loop)(void);
	int sub_devs;
}sDevHnd, *psDevHnd;

/**
 *
 */
sDevHnd devHnd[] = {
	{NULL, NULL, NULL, 0},
	{NULL, sfCmd, NULL, 1},
	{NULL, gpioCmd, NULL, 1},
	{gfxInit, gfxCmd, NULL, 1},
	{i2cInit, i2cCmd, NULL, 2},
	{spiInit, spiCmd, NULL, 1},
	{uartInit, uartCmd, NULL, 1},
	{qrInit, qrCmd, qrLoop, 1},
};

/**
 */
void platformSetup(void)
{
	int i;

	for (i=0; i<sizeof(devHnd)/sizeof(sDevHnd); i++) {
		if (devHnd[i].setup) {
			devHnd[i].setup(devHnd[i].sub_devs);
		}
	}
}

/**
 */
void platformCmd(void)
{
	volatile alt_u32 cmd;

	cmd = *(volatile alt_u32*)DPRAM_BASE;
	if (cmd) {
		int dev;
		dev = MB_DEV(cmd);
		if (dev < sizeof(devHnd)/sizeof(sDevHnd)) {
			if (devHnd[dev].cmd) {
				devHnd[dev].cmd();
			}
		}
		*(volatile alt_u32*)DPRAM_BASE = 0;
	}
}

/**
 */
void platformLoop(void)
{
	int i;

	for (i=0; i<sizeof(devHnd)/sizeof(sDevHnd); i++) {
		if (devHnd[i].loop) {
			devHnd[i].loop();
		}
	}
}
