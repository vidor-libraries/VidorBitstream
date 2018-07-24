/*
 * spi.h
 *
 *  Created on: Apr 18, 2018
 *      Author: max
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
