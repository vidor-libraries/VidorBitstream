/*
 * qr.c
 *
 *  Created on: May 16, 2018
 *      Author: max
 *
 * il registro 0
 *    ha il bit 0 che ti dice se i registri delle posizioni sono aggiornati.
 *    Per azzerarlo è necessario scrivere 1 nel bit 0 del registro 1.
 *    bit 2:1 che decidono la modalità di visualizzazione dell’immagine.
 *       0 è bypass,
 *       1 è ingresso in scala grigi più bordi in evidenza,
 *       2 è una modalità mia di test
 *       3 visualizza solo i bordi
 *
 *
 * Il registro 1
 *    ha i bit 3:0 che contengono la validità delle coordinate.
 *
 *
 *
 *
 */

#include <stdio.h>
#include <io.h>
#include <string.h>
#include <system.h>

#include "mb.h"
#include "qr.h"

#define QR_CNT_MAX 1000

#define QR_STS_BUSY  1
#define QR_STS_READY 2
#define QR_STS_NOQR  3


int qrEnable;
sQrDet qr;
alt_u32 qrCnt;

alt_u32 qrMode(alt_u32 mode);
alt_u32 qrGet(alt_u32* data);

/**
 */
void qrInit(int devs)
{
#if 0
	int *ptr;
	int i;

	ptr = (int*)(640*480*2);
	for (i=0; i<(640*480/2); i++) {
		*ptr++ = 0;
	}
#endif
	qrEnable = 1;
	qrCnt = 0;
}

/**
 */
void qrCmd(void)
{
	alt_u32 volatile *rpc = (alt_u32*)DPRAM_BASE;
	alt_u32 ret;

	ret = -1;
	switch(MB_CMD(rpc[0])){
	case 1:
		/* enable QR Code detection */
		qrEnable = rpc[1];
		ret = 0;
		break;
	case 2:
		/* set display mode */
		ret = qrMode(rpc[1]);
		break;
	case 3:
		/* get qr detect */
		ret = qrGet((alt_u32*)&rpc[2]);
		break;
	}
	rpc[1] = ret;
}

/**
 */
void qrLoop(void)
{
	static int ofs[3] = {0, 0, 0};

	if (qrEnable) {
		int ctrl;
		int stat;
		int i;

		ctrl = IORD(QRCODE_FINDER_0_BASE, 0);
		if (ctrl & 1) {
			stat = IORD(QRCODE_FINDER_0_BASE, 1);
			if (stat == 7) {
				qr.sts = QR_STS_BUSY;
				for (i=0; i<3; i++) {
					qr.pt[i].xs = IORD(QRCODE_FINDER_0_BASE, 2+4*i);
					qr.pt[i].xe = IORD(QRCODE_FINDER_0_BASE, 3+4*i);
					qr.pt[i].ys = IORD(QRCODE_FINDER_0_BASE, 4+4*i);
					qr.pt[i].ye = IORD(QRCODE_FINDER_0_BASE, 5+4*i);
/*
					int *ptr;
					ptr = (int*)(640*480*2);
					ptr[ofs[i]] = 0;
					ofs[i] = ((qr.pt[i].xs+qr.pt[i].xe)/2 + (qr.pt[i].ys+qr.pt[i].ye)/2*640)/2;
					ptr[ofs[i]] = 0xffffffff;
*/
				}
				qr.sts = QR_STS_READY;
				qrCnt = 0;
			}
			IOWR(QRCODE_FINDER_0_BASE, 0, ctrl);
		}
		qrCnt++;
		if (qrCnt > QR_CNT_MAX) {
			qr.sts = QR_STS_NOQR;
			qrCnt = QR_CNT_MAX;
		}
	}
}

/**
 *
 */
alt_u32 qrMode(alt_u32 mode)
{
	int ctrl;

	ctrl = IORD(QRCODE_FINDER_0_BASE, 0);
	ctrl &=~ 0x00000006;
	switch (mode) {
	case 0: ctrl |= 0x00000000; break;
	case 1: ctrl |= 0x00000002; break;
	case 2: ctrl |= 0x00000004; break;
	case 3: ctrl |= 0x00000006; break;
	}
	IOWR(QRCODE_FINDER_0_BASE, 0, ctrl);

	return 0;
}

/**
 *
 */
alt_u32 qrGet(alt_u32* data)
{
	while(qr.sts == QR_STS_BUSY);
	memcpy(data, &qr, sizeof(sQrDet));
	return 0;
}
