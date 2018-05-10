/*
 * gpio.c
 *
 *  Created on: May 7, 2018
 *      Author: max
 */

#include <stdio.h>
#include <io.h>
#include <system.h>

#include "mb.h"

/* GPIO modes */
#define GPIO_NC     0
#define GPIO_IO_I   1
#define GPIO_IO_O   2
#define GPIO_PWM    3
#define GPIO_AUX    4

/* GPIO registers */
#define GPIO_VAL   0x00
#define GPIO_DIR   0x01
#define GPIO_CLR   0x02
#define GPIO_SET   0x03
#define GPIO_MUX0  0x04
#define GPIO_MUX1  0x05

/* PWM registers */
#define PWM_PRESCALER   0
#define PWM_PERIOD_MAX  1
#define PWM_MATCH_H(i)  (2 + (i<<1))
#define PWM_MATCH_L(i)  (2 + (i<<1) + 1)

#if 0
#define PEX_PIO_BASE 0x70010220
#define SAM_PIO_BASE 0x70010200
#define WM_PIO_BASE  0x70010240

#define SAM_PWM_BASE 0x70010300
#endif


alt_u32 gpioPinMode(alt_u32 pin, alt_u32 mode);
alt_u32 gpioWrite(alt_u32 pin, alt_u32 val);
alt_u32 gpioRead(alt_u32 pin);
alt_u32 pwmFrqSet(alt_u32 prescaler, alt_u16 period);
alt_u32 pwmWrite(alt_u32 pin, alt_u16 mh, alt_u16 ml);

/**
 *
 */
void gpioCmd(void)
{
	alt_u32 volatile *rpc = (alt_u32*)DPRAM_BASE;
	alt_u32 ret;

	ret = -1;
	switch(MB_CMD(rpc[0])){
	case 1: ret = gpioPinMode(rpc[1], rpc[2]); break;
	case 2: ret = gpioWrite(rpc[1], rpc[2]); break;
	case 3: ret = gpioRead(rpc[1]); break;
	case 4: ret = pwmFrqSet(rpc[1], rpc[2]); break;
	case 5: ret = pwmWrite(rpc[1], rpc[2], rpc[3]); break;
	}
	rpc[1] = ret;
}

/**
 *
 */
alt_u32 gpioPinMode(alt_u32 pin, alt_u32 mode)
{
	alt_u32 base;
	alt_u32 port;
	alt_u32 pad;
	alt_u32 reg;
	alt_u32 val;

	port = pin >> 5;
	pad = pin & 0x1F;
	switch(port){
	case 0: base = PEX_PIO_BASE; break;
	case 1: base = SAM_PIO_BASE; break;
	case 2: base = WM_PIO_BASE; break;
	default: return -1;
	}

	switch(mode){
	case GPIO_NC:
	case GPIO_IO_I:
		reg = GPIO_MUX0 + (port>>1) + (pad>>4);
		val = IORD(base, reg);
		val &=~ 0x3 << (pad & 0x0F);
		//val |= mode << (pad & 0x0F);
		IOWR(base, reg, val);

		val = IORD(base, GPIO_DIR);
		val &=~ 1 << pad;
		IOWR(base, GPIO_DIR, val);
		break;

	case GPIO_IO_O:
		reg = GPIO_MUX0 + (port>>1) + (pad>>4);
		val = IORD(base, reg);
		val &=~ 0x3 << (pad & 0x0F);
		//val |= mode << (pad & 0x0F);
		IOWR(base, reg, val);

		val = IORD(base, GPIO_DIR);
		val |= 1 << pad;
		IOWR(base, GPIO_DIR, val);
		break;

	case GPIO_PWM:
		reg = GPIO_MUX0 + (port>>1) + (pad>>4);
		val = IORD(base, reg);
		val &=~ 0x3 << (pad & 0x0F);
		val |= 1 << (pad & 0x0F);
		IOWR(base, reg, val);
		break;

	case GPIO_AUX:
		reg = GPIO_MUX0 + (port>>1) + (pad>>4);
		val = IORD(base, reg);
		val &=~ 0x3 << (pad & 0x0F);
		val |= 2 << (pad & 0x0F);
		IOWR(base, reg, val);
		break;
	}
	return 0;
}

/**
 *
 */
alt_u32 gpioWrite(alt_u32 pin, alt_u32 val)
{
	alt_u32 base;
	alt_u32 port;
	alt_u32 pad;

	port = pin >> 5;
	pad = pin & 0x1F;

	switch(port){
	case 0: base = PEX_PIO_BASE; break;
	case 1: base = SAM_PIO_BASE; break;
	case 2: base = WM_PIO_BASE; break;
	default: return -1;
	}

	if(val){
		IOWR(base, GPIO_SET, 1<<pad);
	}else{
		IOWR(base, GPIO_CLR, 1<<pad);
	}
	return 0;
}

/**
 *
 */
alt_u32 gpioRead(alt_u32 pin)
{
	alt_u32 base;
	alt_u32 port;
	alt_u32 pad;
	alt_u32 val;

	port = pin >> 5;
	pad = pin & 0x1F;

	switch(port){
	case 0: base = PEX_PIO_BASE; break;
	case 1: base = SAM_PIO_BASE; break;
	case 2: base = WM_PIO_BASE; break;
	default: return -1;
	}
	val = IORD(base, GPIO_VAL) & (1<<pad);
	return val?1:0;
}

/**
 *
 */
alt_u32 pwmFrqSet(alt_u32 prescaler, alt_u16 period)
{
	IOWR(SAM_PWM_BASE, PWM_PRESCALER, prescaler-1);
	IOWR(SAM_PWM_BASE, PWM_PERIOD_MAX, period-1);
	return 0;
}

/**
 *
 */
alt_u32 pwmWrite(alt_u32 pin, alt_u16 mh, alt_u16 ml)
{
	alt_u32 pad;

	pad = pin & 0x1F;
	IOWR(SAM_PWM_BASE, PWM_MATCH_H(pad), mh);
	IOWR(SAM_PWM_BASE, PWM_MATCH_L(pad), ml);
	return 0;
}
