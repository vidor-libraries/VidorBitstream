/*
 * main.c
 *
 *  Created on: May 29, 2018
 *      Author: max
 */

//#include <altera_generic_quad_spi_controller2.h>
#include <io.h>
#include <string.h>
#include <system.h>
#include <linker.h>

#include "gfx.h"
#include "sign.h"
#include "pio.h"

void platformSetup(void);
void platformCmd(void);
void platformLoop(void);

/**
 *
 */
int main()
{
	int ret;

	// imposta la flash in SPI
	alt_u32 reg;

	reg = IORD(IRQ_BASE, PIO_DIR);
	reg |= (1 << SPI_MODE_PIN);
	IOWR(IRQ_BASE, PIO_DIR , reg);
	IOWR(IRQ_BASE, PIO_CLR, (1 << SPI_MODE_PIN));

	// logo iniziale
	gfxInit(0);

	// calcola la firma
	ret = sign();

	// cancella il codice eseguito fin qui
	memset((void*)BOOT_REGION_BASE, 0, BOOT_REGION_SPAN);

	// verifica la validità della firma
	if(ret){
		while(1);
	}

	platformSetup();
	while (1) {
		platformCmd();
	};

	return 0;
}
