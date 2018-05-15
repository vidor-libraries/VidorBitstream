/*
 * aes.c
 *
 *  Created on: May 10, 2018
 *      Author: max
 */

/*

La memory map è come segue:

registro  descrizione
0         bit 0:1 key mode, bit 2 encode/decode (questo registro si può rileggere)
4:7       dati da codificare/decodificare. Scrivendo il registro 7 parte la codifica/decodifica.
          Rileggendo si ha il dato codificato/decodificato.
          Leggendo il registro 7 si da l’ok alla IP ad emettere un nuovo dato.
8:15      chiave. Scrivendo il registro 15 parte la elaborazione della chiave.

Nota che gli automatismi potrebbero non funzionare…
se leggi deadbeef dai registri di codifica/decodifica
vuol dire che hai fatto troppo presto…

Non mettere polling perché in caso facciamo una cosa diversa.


#define AES_0_BASE 0x70010380
#define AES_0_IRQ -1
#define AES_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define AES_0_NAME "/dev/AES_0"
#define AES_0_SPAN 64
#define AES_0_TYPE "AES"
#define ALT_MODULE_CLASS_AES_0 AES
 */

#include <stdio.h>
#include <io.h>
#include <system.h>

#include "mb.h"

/**
 * AES Registers map
 */
#define AES_CFG     0x00
#define AES_DATA    0x04
#define AES_KEY     0x08

/**
 * Selects the key size mode according to the following list:
 * • 2'b00 value selects 128 bit key length
 * • 2'b01 value selects 192 bit key length
 * • 2'b10 value selects 256 bit key length
 * This value should not change during operation of the core
 * unless the key mode is changed and key expansion is restarted.
 */
#define AES_CFG_KEY_128     0x00
#define AES_CFG_KEY_192     0x01
#define AES_CFG_KEY_256     0x02

#define AES_CFG_ENCODE      0x00
#define AES_CFG_DECODE      0x04

alt_u32 alt_base = 0;//AES_0_BASE;

alt_u32 aesEncode(alt_u32* key, alt_u32* data);
alt_u32 aesDecode(alt_u32* key, alt_u32* data);

void aesConfig(int key_mode, int inc_dec);
void aesKeyWr(alt_u32* key);
void aesDataWr(alt_u32* data);
void aesDataRd(alt_u32* data);

/**
 *
 */
void aesInit(int devs)
{
}

/**
 *
 */
void aesCmd(void)
{
	alt_u32 volatile *rpc = (alt_u32*)DPRAM_BASE;
	alt_u32 ret;

	ret = -1;
	switch(MB_CMD(rpc[0])){
	case 1:
		ret = aesEncode((alt_u32*)rpc[1], (alt_u32*)rpc[9]);
		break;
	case 2:
		ret = aesDecode((alt_u32*)rpc[1], (alt_u32*)rpc[9]);
		break;
	}
	rpc[1] = ret;
}

/**
 *
 */
alt_u32 aesEncode(alt_u32* key, alt_u32* data)
{
	return 0;
}

/**
 *
 */
alt_u32 aesDecode(alt_u32* key, alt_u32* data)
{
	return 0;
}




alt_u32 aesTest(void)
{
	/*
	alt_u32 key[8] = {0x12345678, 0x12345678, 0x12345678, 0x12345678,
			          0x12345678, 0x12345678, 0x12345678, 0x12345678};
	alt_u32 dec[4] = {0x12345678, 0x12345678, 0x12345678, 0x12345678};
	alt_u32 enc[4] = {0, 0, 0, 0};
*/
#if 0
	alt_u8 key[32] = {"1234567890123456790123465798012"};
	alt_u8 plain[16] = {"12345678abcdfegh"};
#endif
#if 0
	alt_u8 key[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	alt_u8 plain[16] = {0x01, 0x47, 0x30, 0xf8, 0x0a, 0xc6, 0x25, 0xfe,
			            0x84, 0xf0, 0x26, 0xc6, 0x0b, 0xfd, 0x54, 0x7d};
#endif


#if 1
/*
	KEY = 8d2e60365f17c7df1040d7501b4a7b5a
	PLAINTEXT = 59b5088e6dadc3ad5f27a460872d5929
	CIPHERTEXT = a02600ecb8ea77625bba6641ed5f5920
*/
	alt_u8 key[32] = {0x8d, 0x2e, 0x60, 0x36, 0x5f, 0x17, 0xc7, 0xdf,
			          0x10, 0x40, 0xd7, 0x50, 0x1b, 0x4a, 0x7b, 0x5a};
	alt_u8 plain[16] = {0x59, 0xb5, 0x08, 0x8e, 0x6d, 0xad, 0xc3, 0xad,
			            0x5f, 0x27, 0xa4, 0x60, 0x87, 0x2d, 0x59, 0x29};
#endif
/*
	KEY = 2d0860dae7fdb0bd4bfab111f615227a
	PLAINTEXT = a02600ecb8ea77625bba6641ed5f5920
	CIPHERTEXT = 5241ead9a89ca31a7147f53a5bf6d96a
*/
/*
	KEY = 7f498a034f6113a73abd442bade3fb10
	PLAINTEXT = 5241ead9a89ca31a7147f53a5bf6d96a
	CIPHERTEXT = 22f09171bc67d0661d1c25f181a69f33
*/



	alt_u8 enc[16];
	alt_u8 new[16];

	aesKeyWr((alt_u32*)key);
	aesDataWr((alt_u32*)plain);
	aesDataRd((alt_u32*)enc);
	aesConfig(AES_CFG_KEY_128, AES_CFG_ENCODE);

	aesKeyWr((alt_u32*)key);
	aesDataWr((alt_u32*)enc);
	aesDataRd((alt_u32*)new);
	aesConfig(AES_CFG_KEY_128, AES_CFG_DECODE);
	return 0;
}







void aesConfig(int key_mode, int inc_dec)
{
	IOWR(alt_base, AES_CFG, key_mode | inc_dec);
}

void aesKeyWr(alt_u32* key)
{
	IOWR(alt_base, AES_KEY+0, key[0]);
	IOWR(alt_base, AES_KEY+1, key[1]);
	IOWR(alt_base, AES_KEY+2, key[2]);
	IOWR(alt_base, AES_KEY+3, key[3]);
	IOWR(alt_base, AES_KEY+4, key[4]);
	IOWR(alt_base, AES_KEY+5, key[5]);
	IOWR(alt_base, AES_KEY+6, key[6]);
	IOWR(alt_base, AES_KEY+7, key[7]);
}

void aesDataWr(alt_u32* data)
{
	IOWR(alt_base, AES_DATA+0, data[0]);
	IOWR(alt_base, AES_DATA+2, data[1]);
	IOWR(alt_base, AES_DATA+3, data[2]);
	IOWR(alt_base, AES_DATA+4, data[3]);
}

void aesDataRd(alt_u32* data)
{
	data[0] = IORD(alt_base, AES_DATA+0);
	data[1] = IORD(alt_base, AES_DATA+2);
	data[2] = IORD(alt_base, AES_DATA+3);
	data[3] = IORD(alt_base, AES_DATA+4);
}
