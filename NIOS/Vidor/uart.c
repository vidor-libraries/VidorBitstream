/*
 * uart.c
 *
 *  Created on: May 8, 2018
 *      Author: max
 */

#include <system.h>

#include "mb.h"

#define UART_DEV_NUM	1

alt_u32 uart_baseaddr[UART_DEV_NUM] = {
		NINA_UART_BASE,
};
/*
 #define NINA_UART_BASE 0x700100c0
 */
/**
 *
 */
int uartCmd(void)
{
	return 0;
}



