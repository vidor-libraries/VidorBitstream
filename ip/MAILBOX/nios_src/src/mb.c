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
 * @file mb.c
 * @brief Mailbox
 */

#include <io.h>
#include "system.h"
#include "mb.h"

#define SEC_RAM  __attribute__((__section__(".rwdata")))

#define MB_DPRAM_BITS   9
#define MB_FIFO_BITS    4

#define MB_LAST_ADDRESS (1 << MB_DPRAM_BITS)
#define MB_FIFO_SIZE    (1 << MB_FIFO_BITS)

#define MB_FIFO_BASE  (MB_LAST_ADDRESS - MB_FIFO_SIZE)
#define MB_STATUS     MB_FIFO_BASE
#define MB_STS_REQ_PEND   (1 << MB_FIFO_BITS)
#define MB_STS_FIFO_FREE  ((1 << MB_FIFO_BITS)-1)

/**
 */
alt_u32* SEC_RAM mbPtrGet(void)
{
  return (alt_u32*)(MB_BASE | 0x80000000);
}

/**
 */
alt_u32* SEC_RAM mbMsgRx(void)
{
  alt_u32 reg;

  reg = IORD(MB_BASE, MB_STATUS);
  if (reg & MB_STS_REQ_PEND) {
    return (alt_u32*)MB_BASE;
  }
  return (alt_u32*)0;
}

/**
 */
alt_u32 SEC_RAM mbEveTx(alt_u32 eve)
{
  alt_u32 reg;

  reg = IORD(MB_BASE, MB_STATUS);
  reg &= MB_STS_FIFO_FREE;
  if (reg) {
    IOWR(MB_BASE, MB_FIFO_BASE, eve);
    return 0;
  }
  return -1;
}
