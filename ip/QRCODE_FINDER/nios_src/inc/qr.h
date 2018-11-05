/*
* Copyright 2018 ARDUINO SA (http://www.arduino.cc/)
* This file is part of Vidor IP.
* Copyright (c) 2018
* Authors: Dario Pennisi
*
* This software is released under:
* The GNU General Public License, which covers the main part of Vidor IP
* The terms of this license can be found at:
* https://www.gnu.org/licenses/gpl-3.0.en.html
*
* You can be released from the requirements of the above licenses by purchasing
* a commercial license. Buying such a license is mandatory if you want to modify or
* otherwise use the software for commercial activities involving the Arduino
* software without disclosing the source code of your own applications. To purchase
* a commercial license, send an email to license@arduino.cc.
*
*/


#ifndef QR_H_
#define QR_H_

#include <alt_types.h>

/**
 * configuration options
 */
#define QR_CNT_MAX      1000
#define QR_PT_NUM       100

#define QR_USE_GFX      0

/**
 * IP configuration
 */
#define QR_UID  0xA2C68
#define QR_IP_VER   0x0302
#define QR_DRV_VER  0x0406
#define QR_VER      (((QR_IP_VER)<<16)|(QR_DRV_VER))

/**
 *
 */
typedef struct {
  alt_u32  xs ;
  alt_u32  xe ;
  alt_u32  ys ;
  alt_u32  ye ;
  alt_u32  valid ;
}sQrPnt, *psQrPnt;

#define QR_PT_DET_NUM 12

/**
 */
typedef struct{
  alt_u32 sts;
  sQrPnt  pt[QR_PT_DET_NUM];
}sQrDet, *psQrDet;

void qrRpc(void);
void qrLoop(void);

#endif /* QR_H_ */
