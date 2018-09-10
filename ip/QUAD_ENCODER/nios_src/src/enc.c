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


#include <string.h>
#include <stdlib.h>
#include <io.h>

#include "config.h"
#include "mb.h"
#include "rpc.h"
#include "enc.h"

alt_u32 encSetup(alt_u32 cmd);
alt_u32 encEnd(alt_u32 cmd);
alt_u32 encGet(alt_u32 cmd);


/**
 */
void encRpc(void)
{
  alt_u32 volatile *rpc = mbPtrGet();
  alt_u32 ret;

  ret = -1;
  if ((fpgaIp[RPC_GIID(rpc[0])].disc & 0xFFFFF) != ENC_UID) {
    rpc[1] = ret;
    return ;
  }
  switch (RPC_PID(rpc[0])) {
  case 2:
    ret = encSetup(rpc[0]);
    break;
  case 4:
    ret = encEnd(rpc[0]);
    break;
  case 5:
    /* Read encoder */
    ret = encGet(rpc[0]);
    break;
  }
  rpc[1] = ret;
}

/**
 */
alt_u32 encSetup(alt_u32 cmd)
{
  return 0;
}

/**
 */
alt_u32 encEnd(alt_u32 cmd)
{
  return 0;
}

/**
 */
alt_u32 encGet(alt_u32 cmd)
{
  alt_u8 giid = RPC_GIID(cmd);
  alt_u16 chn = RPC_CHN(cmd);

  return IORD(fpgaIp[giid].base, chn);
}
