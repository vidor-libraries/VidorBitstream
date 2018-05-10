/*
 * spi.c
 *
 *  Created on: Apr 18, 2018
 *      Author: max
 */



#include <system.h>
#include <altera_avalon_spi.h>
#include <altera_avalon_spi_regs.h>

#include "mb.h"

alt_u32 spi_baseaddr[] = {
	NINA_SPI_BASE,
};

alt_u32 spi_dev_num;

/*
alt_u32 spiTrx(alt_u32 index,
		alt_u32 txl, const alt_u8* txb,
		alt_u32 rxl, alt_u8 * rxb);
*/
alt_u32 spiTrx(alt_u32 index, alt_u8* buf, alt_u32 len);


/**
 *
 */
void spiInit(int devs)
{
	spi_dev_num = devs;
}

/**
 *
 */
void spiCmd(void)
{
	alt_u32 volatile *rpc = (alt_u32*)DPRAM_BASE;
	alt_u32 ret;

	ret = -1;
	switch(MB_CMD(rpc[0])){
	case 1:
		ret = spiTrx(MB_SUB(rpc[0]), (alt_u8*)rpc[2], rpc[1]);
		break;
	case 2:
		ret = spiTrx(MB_SUB(rpc[0]), (alt_u8*)rpc[1], 1);
		break;
	}
	rpc[1] = ret;
}

alt_u32 spiTrx(alt_u32 index, alt_u8* buf, alt_u32 len)
{
	if(index >= spi_dev_num){
		return -1;
	}
	alt_u32 base = spi_baseaddr[index];
	/**
	 * We must not send more than two bytes to the target before it has
	 * returned any as otherwise it will overflow.
	 * Unfortunately the hardware does not seem to work with credits > 1,
	 * leave it at 1 for now.
	 */
	alt_32 credits = 1;
	int i;
	alt_u32 status;

	/**
	 * Discard any stale data present in the RXDATA register, in case
	 * previous communication was interrupted and stale data was left
	 * behind.
	 */
	IORD_ALTERA_AVALON_SPI_RXDATA(base);

	/* Keep clocking until all the data has been processed. */
	for(i=0; i<len; i++){

		do {
			status = IORD_ALTERA_AVALON_SPI_STATUS(base);
		} while (((status & ALTERA_AVALON_SPI_STATUS_TRDY_MSK) == 0 || credits == 0) &&
				(status & ALTERA_AVALON_SPI_STATUS_RRDY_MSK) == 0);

		if ((status & ALTERA_AVALON_SPI_STATUS_TRDY_MSK) != 0 && credits > 0) {
			credits--;
			IOWR_ALTERA_AVALON_SPI_TXDATA(base, *buf++);
		}

		if ((status & ALTERA_AVALON_SPI_STATUS_RRDY_MSK) != 0) {
			*buf++ = (alt_u8)IORD_ALTERA_AVALON_SPI_RXDATA(base);
			credits++;
		}
	}

	/* Wait until the interface has finished transmitting */
	do {
		status = IORD_ALTERA_AVALON_SPI_STATUS(base);
	} while ((status & ALTERA_AVALON_SPI_STATUS_TMT_MSK) == 0);

	return 0;
}
