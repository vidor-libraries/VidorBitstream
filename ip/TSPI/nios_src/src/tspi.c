/*
* Copyright 2018 ARDUINO SA (http://www.arduino.cc/)
* This file is part of Vidor IP.
* Copyright (c) 2018
* Authors: Dario Pennisi
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
*/

#define TSPI_RPC_CMD  1

#include <io.h>


#include "config.h"

#if defined(TSPI_RPC_CMD) && (TSPI_RPC_CMD == 1)
  #include "mb.h"
#endif /* defined(TSPI_RPC_CMD) && (TSPI_RPC_CMD == 1) */

#include "tspi.h"

#define TSPI_RXDATA    0
#define TSPI_TXDATA    1
#define TSPI_STATUS    2
#define TSPI_CONTROL   3
#define TSPI_BAUD      4

#define TSPI_STATUS_TXE 0x1
#define TSPI_STATUS_TXR 0x2

#define TSPI_SS_H    0x04
#define TSPI_SS_L    0x00

#define TSPI_CPHA    0x01
#define TSPI_CPOL    0x02

#define TSPI_MODE_0    (0|0)
#define TSPI_MODE_1    (0|TSPI_CPHA)
#define TSPI_MODE_2    (TSPI_CPOL|0)
#define TSPI_MODE_3    (TSPI_CPOL|TSPI_CPHA)

#if defined(TSPI_RPC_CMD) && (TSPI_RPC_CMD == 1)

/**
 *
 */
void tspiRpc(void)
{
  alt_u32 volatile *rpc = mbPtrGet();
  alt_u32 ret;

  ret = -1;
  if ((fpgaIp[RPC_GIID(rpc[0])].disc & 0xFFFFF) != TSPI_UID) {
    rpc[1] = ret;
    return ;
  }
  switch (RPC_PID(rpc[0])) {
  case 2:
    ret = tspiSetup(rpc[0]);
    break;
  case 4:
    ret = tspiEnd(rpc[0]);
    break;
  case 5:
    ret = tspiModeSet(rpc[0], rpc[1], rpc[2], rpc[3], rpc[4]);
    break;
  case 6:
    ret = tspiTrx(rpc[0], (alt_u8*)&rpc[2], rpc[1]);
    break;
  case 7:
    tspiTrx(rpc[0], (alt_u8*)&rpc[1], 1);
    ret = rpc[1];
    break;
  }
  rpc[1] = ret;
}
#endif /* defined(TSPI_RPC_CMD) && (TSPI_RPC_CMD == 1) */

/**
 */
alt_u32 tspiSetup(alt_u32 cmd)
{
  //alt_u8  giid = RPC_GIID(cmd);
  return 0;
}

/**
 */
alt_u32 tspiEnd(alt_u32 cmd)
{
  return 0;
}

/**
 */
alt_u32 tspiModeSet(alt_u32 cmd, alt_u32 baud, alt_u32 mode, alt_u32 bit_order, alt_u32 ss_auto)
{
  alt_u8    giid = RPC_GIID(cmd);
  psTspiDev pDev = (psTspiDev)fpgaIp[giid].priv;
  alt_u32   base = fpgaIp[giid].base;

  switch (mode) {
  case 0: pDev->mode = TSPI_MODE_0; break;
  case 1: pDev->mode = TSPI_MODE_1; break;
  case 2: pDev->mode = TSPI_MODE_2; break;
  case 3: pDev->mode = TSPI_MODE_3; break;
  default: return -1;
  }
  pDev->bit_order = bit_order;
  pDev->ss_auto   = ss_auto;

  IOWR(base, TSPI_CONTROL, TSPI_SS_H | pDev->mode);
  IOWR(base, TSPI_BAUD, ALT_CPU_FREQ / (baud*2));
  if (pDev->ss_auto) {
    IOWR(base, TSPI_CONTROL, TSPI_SS_H  | pDev->mode);
  }

  return 0;
}

/**
 */
alt_u32 tspiTrx(alt_u32 cmd, alt_u8* buf, alt_u32 len)
{
  alt_u8    giid = RPC_GIID(cmd);
  psTspiDev pDev = (psTspiDev)fpgaIp[giid].priv;
  alt_u32   base = fpgaIp[giid].base;
  alt_u8   *rxb = buf;

  /* set SS */
  if (pDev->ss_auto) {
    IOWR(base, TSPI_CONTROL, TSPI_SS_L | pDev->mode);
  }

  IOWR(base, TSPI_TXDATA, *buf++);
  len--;
  if (len) {
    IOWR(base, TSPI_TXDATA, *buf++);
  }
  while (len--) {
    while (!(IORD(base, TSPI_STATUS) & TSPI_STATUS_TXR));
    IOWR(base, TSPI_TXDATA, *buf++);
    *rxb++ = IORD(base, TSPI_RXDATA);
  }
  while (!(IORD(base, TSPI_STATUS) & TSPI_STATUS_TXE));
  *rxb++ = IORD(base, TSPI_RXDATA);

  /* clear SS */
  if (pDev->ss_auto) {
    IOWR(base, TSPI_CONTROL, TSPI_SS_H | pDev->mode);
  }

  return 0;
}

/**
 */
alt_u32 tspiTrc(alt_u32 cmd, alt_u32 txl, alt_u8* txb, alt_u32 rxl, alt_u8* rxb)
{
  alt_u8    giid = RPC_GIID(cmd);
  psTspiDev pDev = (psTspiDev)fpgaIp[giid].priv;
  alt_u32   base = fpgaIp[giid].base;
  int       i;
  alt_u32   bytes = txl+rxl;

  /* set SS */
  if (pDev->ss_auto) {
    IOWR(base, TSPI_CONTROL, TSPI_SS_L | pDev->mode);
  }

  for (i=0; i<bytes; i++) {
    if (i<txl)
      IOWR(base, TSPI_TXDATA, *txb++);
    else
      IOWR(base, TSPI_TXDATA, 0xff);
    if (i>0) {
      while (!(IORD(base, TSPI_STATUS) & TSPI_STATUS_TXR));
      if (i>txl) {
        *rxb++ = IORD(base, TSPI_TXDATA);
      }
    }
  }
  while (!(IORD(base, TSPI_STATUS) & TSPI_STATUS_TXE));
  if (rxl) {
    *rxb++ = IORD(base, TSPI_RXDATA);
  }

  /* clear SS */
  if (pDev->ss_auto) {
    IOWR(base, TSPI_CONTROL, TSPI_SS_H | pDev->mode);
  }
  return 0;
}
