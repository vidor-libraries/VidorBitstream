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
 */

#ifndef TSPI_H_
#define TSPI_H_

#include <alt_types.h>

/**
 * IP configuration
 */
#define TSPI_UID      0xEBCE5
#define TSPI_IP_VER   0x0000
#define TSPI_DRV_VER  0x0000
#define TSPI_VER      (((TSPI_IP_VER)<<16)|(TSPI_DRV_VER))

#define TSPI_PIN_FNC_MOSI PIN_FNC(0x01, PIN_DIR_O)
#define TSPI_PIN_FNC_MISO PIN_FNC(0x02, PIN_DIR_I)
#define TSPI_PIN_FNC_CLK  PIN_FNC(0x03, PIN_DIR_O)
#define TSPI_PIN_FNC_SS   PIN_FNC(0x04, PIN_DIR_O)

/**
 * private data structure definition
 */
typedef struct tspi_dev_s{
  alt_u8    ss_auto;
  alt_u8    mode;
  alt_u8    bit_order;
} sTspiDev, *psTspiDev;

#define TSPI_DEV(name) sTspiDev name##_DEV

void tspiRpc(void);

alt_u32 tspiSetup(alt_u32 cmd);
alt_u32 tspiModeSet(alt_u32 cmd, alt_u32 baud, alt_u32 mode, alt_u32 bit_order, alt_u32 ss_auto);
alt_u32 tspiEnd(alt_u32 cmd);
alt_u32 tspiTrx(alt_u32 cmd, alt_u8* buf, alt_u32 len);
alt_u32 tspiTrc(alt_u32 cmd, alt_u32 txl, alt_u8* txb, alt_u32 rxl, alt_u8* rxb);

#endif /* TSPI_H_ */
