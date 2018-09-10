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
 * @file mb.h
 * @brief Mailbox
 */
#ifndef __MB_H
#define __MB_H

#include "config.h"

/**
 *
 */
#define MB_BASE (0x80000000 | DPRAM_BASE)


int mbInit(void);
alt_u32* mbPtrGet(void);


alt_u32 mbCmdSend(alt_u32* data, alt_u32 len);
alt_u32 mbRead(alt_u32 address, alt_u32* data, alt_u32 len);

#endif /* __MB_H */
