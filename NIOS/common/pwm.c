/*
 * pwm.c
 *
 *  Created on: Aug 29, 2018
 *      Author: max
 */

#include "config.h"

#if defined(PWM_MODULE) && (PWM_MODULE == 1)

#include <alt_types.h>
#include <io.h>

#include "mb.h"
#include "pwm.h"

/* PWM registers */
#define PWM_PRESCALER   0
#define PWM_PERIOD_MAX  1
#define PWM_MATCH_H(i)  (2 + (i<<1))
#define PWM_MATCH_L(i)  (2 + (i<<1) + 1)

/**
 *
 */
void pwmCmd(void)
{
  alt_u32 volatile *rpc = (alt_u32*)MB_BASE;
  alt_u32 ret;

  ret = -1;
  switch(MB_CMD(rpc[0])){
  case 1: ret = pwmFrqSet(rpc[1], rpc[2]); break;
  case 2: ret = pwmWrite(rpc[1], rpc[2], rpc[3]); break;
//  case 2: ret = pwmWrite(MB_CHN(rpc[0]), rpc[1], rpc[2]); break;
  }
  rpc[1] = ret;
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

#endif /* defined(PWM_MODULE) && (PWM_MODULE == 1) */
