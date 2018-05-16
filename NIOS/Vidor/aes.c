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

#ifdef AES

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
#endif  // AES
