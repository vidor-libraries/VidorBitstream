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

#ifndef SPI_H_
#define SPI_H_

#include <alt_types.h>

void spiInit(int);
void spiCmd(void);

alt_u32 spiEnable(alt_u32 idx);
alt_u32 spiModeSet(alt_u32 idx, alt_u32 baud, alt_u32 mode, alt_u32 bitOrder);
alt_u32 spiDisable(alt_u32 idx);
alt_u32 spiTrx(alt_u32 idx, alt_u8* buf, alt_u32 len);
alt_u32 spiTrc(alt_u32 idx, alt_u32 txl, alt_u8* txb, alt_u32 rxl, alt_u8* rxb);

#endif /* SPI_H_ */
