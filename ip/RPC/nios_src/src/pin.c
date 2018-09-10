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
 * @file pin.c
 * @brief fpga pins management
 *
 */

#include "config.h"
#include "rpc.h"

/**
 */
alt_u32 pinSearch(alt_u32 UID, alt_u16* pins)
{/*
  int i;

// TODO deve andare per gruppo di pins

  for (i=0; i<GIID_MAX; i++) {
    if ((fpgaIp[i].disc & 0xFFFFF) == UID) {
      int p;
      int g;

      g = -1;
      for (p=0; p<fpgaIp[i].npin; p++) {
        if (g != fpgaIp[i].pin[p].grp) {
          g == fpgaIp[i].pin[p].grp;
        }
        if (fpgaIp[i].pin[p].pin != pins[p]) {
          return -1;
        }
      }
      // TODO locks pins to giid
      return i;
    }
  }*/
  return -1;
}

/**
 */
alt_u32 pinCheck()
{
  return 0;
}

/**
 */
alt_u32 pinLock()
{
  return 0;
}

/**
 */
alt_u32 pinFree()
{
  return 0;
}
