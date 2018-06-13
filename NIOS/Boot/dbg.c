/*
 * dbg.c
 *
 *  Created on: Jun 7, 2018
 *      Author: max
 */


#include "mb.h"

void dbgSf(void)
{
	alt_u32 volatile *rpc = (alt_u32*)MB_BASE;

	rpc[0] = MB_DEV_SF | 0x01;
	platformCmd();

/*
	// read
	memset(rpc, 0, 32);
	rpc[0] = MB_DEV_SF | 0x05;
	rpc[1] = 0;
	rpc[2] = 16;
	platformCmd();

	// read
	memset(rpc, 0, 32);
	rpc[0] = MB_DEV_SF | 0x05;
	rpc[1] = 64*1024;
	rpc[2] = 16;
	platformCmd();
*/
/*
	// erase first 64K
	rpc[0] = MB_DEV_SF | 0x03;
	rpc[1] = 2;
	rpc[2] = 0;
	platformCmd();

	// erase second 64K
	rpc[0] = MB_DEV_SF | 0x03;
	rpc[1] = 2;
	rpc[2] = 64*1024;
	platformCmd();
*/
	// erase all FLASH
	rpc[0] = MB_DEV_SF | 0x03;
	rpc[1] = 3;
	rpc[2] = 0;
	platformCmd();
//return;
	// program
	rpc[0] = MB_DEV_SF | 0x04;
	rpc[1] = 0;
	rpc[2] = 16;
	strcpy(&rpc[3], "01234567890abcdef");
	platformCmd();

	rpc[0] = MB_DEV_SF | 0x04;
	rpc[1] = 32;
	rpc[2] = 16;
	strcpy(&rpc[3], "01234567890abcdef");
	platformCmd();

	// program
	rpc[0] = MB_DEV_SF | 0x04;
	rpc[1] = 64*1024;
	rpc[2] = 16;
	strcpy(&rpc[3], "fedcba9876543210");
	platformCmd();

	// read
	rpc[0] = MB_DEV_SF | 0x05;
	memset(rpc, 0, 32);
	rpc[1] = 0;
	rpc[2] = 32;
	platformCmd();

	// read
	rpc[0] = MB_DEV_SF | 0x05;
	memset(rpc, 0, 32);
	rpc[1] = 64*1024;
	rpc[2] = 32;
	platformCmd();
}




