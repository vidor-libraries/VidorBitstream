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
 * @file fpga.h
 * @brief
 */

#ifndef FPGA_H_
#define FPGA_H_

#define FPGA_UID 0x000000
#define FPGA_VER 0x01020305

void fpgaRpc(void);

alt_u32 fpgaIpQuery(alt_u32 cmd, alt_u32* pins);
alt_u32 fpgaIpEnd(alt_u32 cmd);

#endif /* FPGA_H_ */
