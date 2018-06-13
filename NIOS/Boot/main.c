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

//#include "dbg.h"
alt_u32 pltAppJmp(alt_u32 entry);

/**
 *
 */
int main()
{
	int ret;
//dbgSf();
//ret = sfJedecId();

	// calcola la firma
	ret = sign();

	// cancella il codice eseguito fin qui deve cancellare text2
	//memset((void*)BOOT_REGION_BASE, 0, BOOT_REGION_SPAN);

	// verifica la validità della firma
/*	if(ret){
		while(1);
	}
*/

	// logo iniziale
	gfxInit(0);
//ruLoad(0x00060000);

	platformSetup();
	while (1) {
		platformCmd();
	};

	return 0;
}
