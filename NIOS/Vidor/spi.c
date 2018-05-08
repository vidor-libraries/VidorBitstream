/*
 * spi.c
 *
 *  Created on: Apr 18, 2018
 *      Author: max
 */



#include <system.h>
#include <altera_avalon_spi.h>

#include "mb.h"

#define SPI_DEV_NUM	1

alt_u32 spi_baseaddr[SPI_DEV_NUM] = {
	NINA_SPI_BASE,
};

/**
 *
 */
int spiCmd(void)
{
	alt_u32 volatile *rpc = (alt_u32*)DPRAM_BASE;
	alt_u32 ret;

	ret = -1;
	switch(MB_CMD(rpc[0])){/*
	case 1: ret = gpioPinMode(rpc[1], rpc[2]); break;
	case 2: ret = gpioWrite(rpc[1], rpc[2]); break;
	case 3: ret = gpioRead(rpc[1]); break;
	case 4: ret = pwmFrqSet(rpc[1], rpc[2]); break;
	case 5: ret = pwmWrite(rpc[1], rpc[2], rpc[3]); break;*/
	}
	rpc[1] = ret;
	return 0;

	return 0;
}

/**
 *
 */
int _______spiCmd(alt_u32 base, alt_u32 slave,
		alt_u32 txl, const alt_u8* txb,
		alt_u32 rxl, alt_u8 * rxb)
{
	return alt_avalon_spi_command(base, slave, txl, txb, rxl, rxb, 0);
}


alt_u32 spi___(alt_u32 index)
{
	if(index >= SPI_DEV_NUM){
		return -1;
	}
	alt_u32 baseaddr = spi_baseaddr[index];

}
