/*
 * main.c
 *
 *  Created on: May 29, 2018
 *      Author: max
 */

#include <linker.h>
#include "gfx.h"
#include "sign.h"

/**
 *
 */
int main()
{
	// verifica firma
	int ret;

	gfxInit(0);

	ret = sign();
	memset(BOOT_REGION_BASE, 0, BOOT_REGION_SPAN);
	// TODO deve cancellare il codice eseguito fin qui
	if(ret){
		while(1);
	}

	for(ret=0; ret<50; ret++){
		fillRect(5+ret*20, 50, 10, 10, 0xFC00);
	}
	return 0;
}
