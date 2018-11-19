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

#ifndef PIO_H_
#define PIO_H_

/**
 * IP configuration
 */
#define PIO_UID       0xE0C45
#define FIO_UID       0xE0C46     /* Fixed IO */
#define PIO_IP_VER    0x0000
#define PIO_DRV_VER   0x0000
#define PIO_VER       (((PIO_IP_VER)<<16)|(PIO_DRV_VER))

#define PIO_PIN_FNC_OUT   PIN_FNC(0x01, PIN_DIR_O)

void pioRpc(void);

alt_u32 pioMode(alt_u8 port, alt_u8 pin, alt_u8 mux, alt_u8 dir, alt_u8 val);

#endif /* PIO_H_ */
