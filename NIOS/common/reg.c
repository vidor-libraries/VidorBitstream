/*
 * sf.c
 *
 *  Created on: Apr 11, 2018
 *      Author: max
 */
#include <alt_types.h>
#include <io.h>

#include "mb.h"

alt_u32 regWr(alt_u32 adr, alt_u32 val);
alt_u32 regRd(alt_u32 adr);

/**
 */
void regCmd(void)
{
  alt_u32 volatile *rpc = (alt_u32*)MB_BASE;
  alt_u32 ret;

  ret = -1;
  switch(MB_CMD(rpc[0])){
  case 1:
    ret = regWr(rpc[1], rpc[2]);
    break;
  case 2:
    ret = regRd(rpc[1]);
    break;
  }
  rpc[1] = ret;
}

/**
 */
alt_u32 regWr(alt_u32 adr, alt_u32 val)
{
  IOWR(adr, 0, val);
  return 0;
}

/**
 */
alt_u32 regRd(alt_u32 adr)
{
  return IORD(adr, 0);
}
