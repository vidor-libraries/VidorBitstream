/**
 *
 * @file enc.c
 * @brief Quad encoder driver
 *
 */

#include <string.h>
#include <stdlib.h>
#include <io.h>

#include <system.h>

#include "config.h"
#include "mb.h"

#define ENC_DEV_NUM      11

alt_u32 encGet(alt_u32 index);

/**
 */
void encInit(int devs)
{
}

/**
 */
void encCmd(void)
{
  alt_u32 volatile *rpc = (alt_u32*)MB_BASE;
  alt_u32 ret;

  ret = -1;
  switch(MB_CMD(rpc[0])){
  case 1:
    /* Read encoder */
    ret = encGet(MB_SUB(rpc[0]));
    break;
  }
  rpc[1] = ret;
}

/**
 */
alt_u32 encGet(alt_u32 index)
{
  if (index >= ENC_DEV_NUM) {
    return -1;
  }
  return IORD(QUAD_ENCODER_0_BASE, index);
}
