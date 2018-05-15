/*
 * mb.c
 *
 *  Created on: Apr 11, 2018
 *      Author: max
 */


#include <alt_types.h>
#include <system.h>
#include <sys/alt_irq.h>

// Interupt pin from SAM
// SAM_PORTS.PB10 IO_B3_R3
#define MB_IRQ IRQ_IRQ

// Interupt pin to SAM
// SAM_PORTS.PB11 IO_B3_T3

void mbIsr(void* isr_context, alt_u32 id);

void (*mbHook)(alt_u32);

/**
 *
 */
/*
int mbInit(void (*hook)(alt_u32))
{
	void* context = 0;
	int ret;

	ret = alt_irq_register(MB_IRQ, context, mbIsr);
	if (ret) {
		return -1;
	}

	mbHook = hook;

	return 0;
}
*/

/**
 *
 */
int mbTxSend(alt_u32 cmd, alt_u32 ptr)
{
	return 0;
}

/**
 *
 */
alt_u32 mbPoll(void)
{
	return *(volatile alt_u32*)DPRAM_BASE;
}

/**
 *
 */
void mbDone(void)
{
	*(volatile alt_u32*)DPRAM_BASE = 0;
}

/**
 *
 */
void mbIsr(void* isr_context, alt_u32 id)
{
	if (mbHook) {
		mbHook(*(volatile alt_u32*)DPRAM_BASE);
	}
}
