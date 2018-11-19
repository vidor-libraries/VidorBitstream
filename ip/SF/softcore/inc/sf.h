/**
 * Copyright 2018 ARDUINO SA (http://www.arduino.cc/)
 * This file is part of Vidor IP.
 * Copyright (c) 2018
 * Authors: Masssimiliano Agneni
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
 */

#ifndef SF_H_
#define SF_H_

/**
 * IP configuration
 */
#define SF_UID       0x5A5D6
#define SF_IP_VER    0x0000
#define SF_DRV_VER   0x0000
#define SF_VER       (((SF_IP_VER)<<16)|(SF_DRV_VER))

#include <alt_types.h>

void sfRpc(void);

void sfInit(int devs);

alt_u32 sfJedecId(void);
alt_u32 sfUniqueId(alt_u8* id);

alt_u32 sfErase(alt_u32 mode, alt_u32 adr);
alt_u32 sfProgram(alt_u32 adr, alt_u8* data, alt_u32 len);
alt_u32 sfRead(alt_u32 adr, alt_u8* data, alt_u32 len);

alt_u32 sfSRErase(alt_u8 reg);
alt_u32 sfSRProgram(alt_u8 reg, alt_u8 adr, alt_u8* data, alt_u32 len);
alt_u32 sfSRLock(alt_u8 reg);
alt_u32 sfSRRead(alt_u8 reg, alt_u8 adr, alt_u8* data, alt_u32 len);

alt_u32 sfProtect(void);

alt_u32 sfEnableQuad(void);

#endif /* SF_H_ */
