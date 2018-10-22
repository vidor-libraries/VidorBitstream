/**
 * Copyright 2018 ARDUINO SA (http://www.arduino.cc/)
 * This file is part of Vidor IP.
 * Copyright (c) 2018
 * Authors: Massimiliano Agneni
 *
 * This software is released under:
 * The GNU General Public License, which covers the main part of Vidor IP
 * The terms of this license can be found at:
 * https://www.gnu.org/licenses/gpl-3.0.en.html
 *
 * You can be released from the requirements of the above licenses by purchasing
 * a commercial license. Buying such a license is mandatory if you want to modify or
 * otherwise use the software for commercial activities involving the Arduino
 * software without disclosing the source code of your own applications. To purchase
 * a commercial license, send an email to license@arduino.cc.
 *
 * @file pwm.c
 * @brief Pulse Width Modulation driver
 */

#include "config.h"

#include <sys/alt_irq.h>
#include <io.h>

#include "mb.h"
#include "rpc.h"
#include "pwm.h"

/* PWM registers */
#define PWM_PRESCALER   0
#define PWM_PERIOD_MAX  1
#define PWM_MATCH_H(i)  (2 + (i<<1))
#define PWM_MATCH_L(i)  (2 + (i<<1) + 1)

alt_u32 pwmSetup(alt_u32 cmd);
alt_u32 pwmEnd(alt_u32 cmd);

/**
 *
 */
void pwmRpc(void)
{
  alt_u32 volatile *rpc = mbPtrGet();
  alt_u32 ret;

  ret = -1;
  if ((fpgaIp[RPC_GIID(rpc[0])].disc & 0xFFFFF) != PWM_UID) {
    rpc[1] = ret;
    return ;
  }
  switch (RPC_PID(rpc[0])) {
  case 2:
    ret = pwmSetup(rpc[0]);
    break;
  case 4:
    ret = pwmEnd(rpc[0]);
    break;
  case 5:
    ret = pwmFrqSet(rpc[1], rpc[2]);
    break;
  case 6:
    ret = pwmWrite(rpc[0], rpc[1], rpc[2]);
    break;
  }
  rpc[1] = ret;
}

/**
 */
alt_u32 pwmSetup(alt_u32 cmd)
{
  return 0;
}

/**
 */
alt_u32 pwmEnd(alt_u32 cmd)
{
  return 0;
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
alt_u32 pwmWrite(alt_u32 cmd, alt_u16 mh, alt_u16 ml)
{
  alt_u8  giid = RPC_GIID(cmd);
  alt_u16 chn  = RPC_CHN(cmd);
  alt_u32 base;

  base = fpgaIp[giid-1].base;
  IOWR(base, PWM_MATCH_H(chn), mh);
  IOWR(base, PWM_MATCH_L(chn), ml);
  return 0;
}
