/*
 * qr.h
 *
 *  Created on: May 16, 2018
 *      Author: max
 */

#ifndef QR_H_
#define QR_H_

#include <alt_types.h>

/**
 *
 */
typedef struct{
	alt_u32 sts;
	struct {
		alt_u32 xs;
		alt_u32 xe;
		alt_u32 ys;
		alt_u32 ye;
	}pt[3];
}sQrDet, *psQrDet;

void qrInit(int devs);
void qrCmd(void);
void qrLoop(void);

#endif /* QR_H_ */
