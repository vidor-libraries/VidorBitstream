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
 * @file config.h
 * @brief FPGA configuration
 */
#ifndef __CONFIG_H
#define __CONFIG_H

#include <system.h>

#include "rpc.h"

extern sFpgaPin fpgaPin[];
extern int fpgaPinsNum;
#define FPGA_PINS_NUM fpgaPinsNum

extern sFpgaIpRpc fpgaIpRpc[];
extern sFpgaIpLoop fpgaIpLoop[];
extern int fpgaIpLoopNum;
#define LOOP_MAX  fpgaIpLoopNum

extern sFpgaIp fpgaIp[];
extern int fpgaIpGiidNum;
#define GIID_MAX  fpgaIpGiidNum

#endif /* __CONFIG_H */

