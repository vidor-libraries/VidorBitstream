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

#include "rpc.h"
#include "mb.h"

alt_u32* mbPtrGet(void)
{
  return (alt_u32*)MB_BASE;
}

/**
 */
int mbInit(void)
{
  //irqPinSet(0, NULL);
  return 0;
}

/**
 */
void mbLoop(void)
{
  //if (irqPinDet(0)) {
  //  irqPinClr(0);
    rpcCmd();
  //}
}

