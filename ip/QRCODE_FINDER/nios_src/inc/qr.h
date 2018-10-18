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

#define QR_UID  0xa2C68


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

#endif /* QR_H_ */
