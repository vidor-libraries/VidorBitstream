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
#include "platform.h"

/**
 *
 */
int main()
{
	int ret;

ruLoad(0x00080000);
	// calcola la firma
	ret = signChk();

	// cancella il codice eseguito fin qui (text2 in boot)
	memset((void*)BOOT_REGION_BASE, 0, BOOT_REGION_SPAN);

	// verifica la validità della firma
	if(ret){
		while(1);
	}

	// logo iniziale
	gfxInit(0);

	platformSetup();
	while (1) {
		platformCmd();
	};

	return 0;
}
