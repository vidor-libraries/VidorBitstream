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

#include "config.h"

#if defined(ENC_MODULE) && (ENC_MODULE == 1)

#include <string.h>
#include <stdlib.h>
#include <io.h>

#include "mb.h"

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
  return IORD(ENC_BASE, index);
}

#endif /* defined(ENC_MODULE) && (ENC_MODULE == 1) */
