/*
 * tmr.c
 *
 *  Created on: Jun 12, 2018
 *      Author: max
 */

#include "config.h"

#include <stdio.h>
#include <io.h>
#include <sys/alt_irq.h>
#include <system.h>


#define TMR_IRQ_CTR_ID  0


/**
 */
#define TMR_REG_STS 0   // status  RW  ...                     RUN     TO
#define TMR_REG_CTR 1   // control RW  ... STOP     START     CONT    ITO
#define TMR_REG_TPL 2   // periodl RW  Timeout Period     bits [15: 0]
#define TMR_REG_TPH 3   // periodh RW  Timeout Period     bits [31:16]
#define TMR_REG_CSL 4   // snapl   RW  Counter Snapshot   bits [15: 0]
#define TMR_REG_CSH 5   // snaph   RW  Counter Snapshot   bits [31:16]

#define TMR_CTR_ITO   0x00000001
#define TMR_CTR_CONT  0x00000002
#define TMR_CTR_START 0x00000004
#define TMR_CTR_STOP  0x00000008

void (*tmrHook)(void*);

#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
void tmrIsr(void* isr_context);
#else
void tmrIsr(void* isr_context, alt_u32 id);
#endif

/**
 *
 */
alt_u32 tmrStart(alt_u32 ms, void (*hook)(void*), void* arg)
{
  alt_u32 ctr;
  int     ret;
  alt_u32 ticks_x_msec = TIMER_0_FREQ / 1000;

  tmrHook = hook;
  if (hook) {
#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
    ret = alt_ic_isr_register(TMR_IRQ_CTR_ID, TIMER_0_IRQ, tmrIsr, arg, 0);
#else
    ret = alt_irq_register(TIMER_0_IRQ, arg, tmrIsr);
#endif
    ctr = TMR_CTR_START | TMR_CTR_ITO;
  } else {
#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
    ret = alt_ic_irq_disable(TMR_IRQ_CTR_ID, TIMER_0_IRQ);
#else
    ret = alt_irq_register(TIMER_0_IRQ, NULL, NULL);
#endif
    ctr = TMR_CTR_STOP;
  }
  if (ret) {
    return -1;
  }

  ms = ms * ticks_x_msec;
  /* set timeout period */
  IOWR(TIMER_0_BASE, TMR_REG_TPL, (alt_u16)(ms & 0x0000FFFF));
  IOWR(TIMER_0_BASE, TMR_REG_TPH, (alt_u16)(ms >> 16));

  /* start or stop timer */
  IOWR(TIMER_0_BASE, TMR_REG_CTR, ctr);

  return 0;
}

#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
void tmrIsr(void* isr_context)
#else
void tmrIsr(void* isr_context, alt_u32 id)
#endif
{
  if (tmrHook) {
    tmrHook(isr_context);
  }
  IOWR(TIMER_0_BASE, TMR_REG_STS, 0);
}
