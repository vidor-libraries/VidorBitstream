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
 * @file rpc.c
 * @brief Remote Procedure Call
 *
 *
 * Table 1. Parameter 0 - Procedure ID
 *  31:24  23:12  11:0
 *  giid   chn    prcid
 *
 *  giid    Global Instance ID
 *  chn     Channel
 *  prcid   Procedure ID
 *
 */

#include "config.h"
#include "mb.h"
#include "fpga.h"

#define SEC_RAM  __attribute__((__section__(".rwdata")))

/**
 */
void SEC_RAM rpcCmd(void)
{
  volatile alt_u32 *rpc = mbPtrGet();

  if (rpc[0]) {
    alt_u8   giid = RPC_GIID(rpc[0]);

    if (giid >= GIID_MAX ) {
      rpc[1] = RPC_ERR_GIID_INV;
      rpc[0] = 0;
      return;
    }
    if (giid) {
      switch (RPC_PID(rpc[0])) {
      case 1:
        rpc[1] = fpgaIpRpc[fpgaIp[giid].rpc].ver;
        rpc[0] = 0;
        return;
      case 2:
        //setup
        break;
      case 3:
        rpc[1] = fpgaIpQuery(rpc[0], (alt_u32*)&rpc[2]);
        rpc[0] = 0;
        return;
      }
    }
    if (fpgaIpRpc[fpgaIp[giid].rpc].rpc) {
      fpgaIpRpc[fpgaIp[giid].rpc].rpc();
    } else {
      rpc[1] = RPC_ERR_GIID_INV;
      rpc[0] = 0;
      return;
    }
    if (giid) {
      switch (RPC_PID(rpc[0])) {
      case 4:
        if (!rpc[1]) {
          fpgaIpEnd(rpc[0]);
        }
        break;
      }
    }
    rpc[0] = 0;
  }
}
