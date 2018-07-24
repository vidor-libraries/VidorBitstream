/*
 * sdram.c
 *
 *  Created on: May 22, 2018
 *      Author: max
 */

#include "config.h"

#if defined(SDRAM_MODULE) && (SDRAM_MODULE == 1)

#include <stdio.h>
#include <string.h>

#include "mb.h"

alt_u32 sdramWr(alt_u32 adr, alt_u8* data, alt_u32 len);
alt_u32 sdramRd(alt_u32 adr, alt_u8* data, alt_u32 len);

/**
 */
void sdramInit(int devs)
{
}

/**
 */
void sdramCmd(void)
{
  alt_u32 volatile *rpc = (alt_u32*)MB_BASE;
  alt_u32 ret;

  ret = -1;
  switch(MB_CMD(rpc[0])){
  case 1:
    /* write */
    ret = sdramWr(rpc[1], (alt_u8*)&rpc[3], rpc[2]);
    break;
  case 2:
    /* read */
    ret = sdramRd(rpc[1], (alt_u8*)&rpc[3], rpc[2]);
    break;
  }
  rpc[1] = ret;
}


/**
 *
 */
alt_u32 sdramWr(alt_u32 adr, alt_u8* data, alt_u32 len)
{
  if ((adr+len) > SDRAM_SIZE) {
    return -1;
  }
  memcpy(SDRAM_BASE, data, len);
  return 0;
}

/**
 *
 */
alt_u32 sdramRd(alt_u32 adr, alt_u8* data, alt_u32 len)
{
  if ((adr+len) > SDRAM_SIZE) {
    return -1;
  }
  memcpy(data, SDRAM_BASE, len);
  return 0;
}

#endif /* defined(SDRAM_MODULE) && (SDRAM_MODULE == 1) */
