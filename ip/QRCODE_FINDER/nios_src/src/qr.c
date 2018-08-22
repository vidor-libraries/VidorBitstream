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

#include <stdio.h>
#include <io.h>
#include <string.h>
#include <system.h>

#include "mb.h"
#include "gfx.h"
#include "qr.h"

#define QR_CNT_MAX 1000

#define QR_STS_BUSY  1
#define QR_STS_READY 2
#define QR_STS_NOQR  3

#define QR_GET_TIMEOUT 10000

//#define QR_DEBUG

#define QR_PT_NUM 100

#define SEC_RAM  __attribute__((__section__(".rwdata")))

#ifdef QR_DEBUG
#include "ov5647.h"
#endif

int     qrEnable;
int     qrDraw;
sQrDet  qr;
alt_u32 qrCnt;

alt_u32 qrMode(alt_u32 mode);
alt_u32 qrGet(alt_u32* data);
alt_u32 qrThrSet(alt_u32 data);

/**
 */
void qrInit(int devs)
{
#ifdef QR_DEBUG
  /*
  int *ptr;
  int i;

  ptr = (int*)((SDRAM_ARBITER_BASE + SDRAM_ARBITER_FB_OFFSET*sizeof(short)) |
               0x80000000);

  for (i=0; i<(640*480/2); i++) {
    *ptr++ = 0;
  }
  */
  ov5647SensorPower(1);
  ov5647StreamOn();
  qrEnable = 1;
  qrDraw = 1;
#else
  qrEnable = 0;
  qrDraw = 0;
#endif
  //qrThrSet(120);
  qr.sts = QR_STS_NOQR;
  qrCnt = 0;
}

/**
 */
void qrCmd(void)
{
  alt_u32 volatile *rpc = (alt_u32*)MB_BASE;
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
  case 4:
    /* set qr threshold */
    ret = qrThrSet(rpc[1]);
    break;
  case 5:
    /* set cross drawing */
    qrDraw = rpc[1];
    break;
  }
  rpc[1] = ret;
}

/**
 */
void qrCross(alt_u32 x, alt_u32 y, alt_u32 c)
{
   if (x<4) x=4;
   if (y<4) y=4;
   writeLine(x-4, y, x+4, y, c);
   writeLine(x, y-4, x, y+4, c);
}

/**
 */
void SEC_RAM qrLoop(void)
{
  alt_u32 temp, ty, txs, txe, tc, tmaxy;
  sQrPnt  pt[QR_PT_NUM];

  if (qrEnable) {
    int ctrl;
    int i, j;
    int gc;
    ctrl = IORD(QRCODE_FINDER_0_BASE, 0);
    if (ctrl & 1) {
      qr.sts = QR_STS_BUSY;
      tmaxy = 0;
      for(i=0; i<QR_PT_NUM; i++){
        pt[i].valid = 0;
      }
      for(i=0; i<1024; i++){
        temp = IORD(QRCODE_FINDER_0_BASE, 1024+i);
        if (temp==0xffffffff){
          break;
        }
        txe =  temp      & 0x3ff;
        txs = (temp>>10) & 0x3ff;
        ty  = (temp>>20) & 0x3ff;
        tc = (txs+txe)/2;
        gc =-1;
        for (j=0; j<QR_PT_NUM; j++) {
          if (pt[j].valid) {
            if ((tc >= pt[j].xs) && (tc <= pt[j].xe) && ((ty-pt[j].ye)<4)) {
              pt[j].ye = ty;
              if (tmaxy < (pt[j].ye-pt[j].ys)) {
                tmaxy = (pt[j].ye-pt[j].ys);
              }
              break;
            }
            else if ((ty-pt[j].ye)>4 && (pt[j].ye-pt[j].ys)<2) {
              pt[j].valid = 0;
              gc=j;
            }
          } else {
            break;
          }
        }
        if (j<QR_PT_NUM && !pt[j].valid) {
          pt[j].valid = 1;
          pt[j].xs = txs;
          pt[j].xe = txe;
          pt[j].ys = ty;
          pt[j].ye = ty;
        }
        else if (j==QR_PT_NUM && gc) {
          pt[gc].valid = 1;
          pt[gc].xs = txs;
          pt[gc].xe = txe;
          pt[gc].ys = ty;
          pt[gc].ye = ty;
        }
      }

      for (i=0; i<QR_PT_DET_NUM; i++) {
        if (qrDraw) {
          if (qr.pt[i].valid==1) {
            qrCross((qr.pt[i].xe+qr.pt[i].xs)/2, (qr.pt[i].ye+qr.pt[i].ys)/2, 0);
          }
        }
        qr.pt[i].valid = 0;
      }

      j = 0;
      for (i=0; i<QR_PT_NUM; i++) {
        if (pt[i].valid && (pt[i].ye-pt[i].ys)<(tmaxy/2)) {
          pt[i].valid = 0;
        } else if (pt[i].valid) {
          qr.pt[j].xs = pt[i].xs;
          qr.pt[j].xe = pt[i].xe;
          qr.pt[j].ys = pt[i].ys;
          qr.pt[j].ye = pt[i].ye;
          pt[i].valid = 0;
          qr.pt[j].valid = 1;
          if (qrDraw) {
            qrCross((qr.pt[j].xe+qr.pt[j].xs)/2, (qr.pt[j].ye+qr.pt[j].ys)/2, 0xffff);
          }
          j++;
          if (j==QR_PT_DET_NUM) {
            break;
          }
        }
      }
      qr.sts = QR_STS_READY;
      qrCnt = 0;
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
alt_u32 SEC_RAM qrGet(alt_u32* data)
{
  if (qr.sts == QR_STS_BUSY) {
    return -1;
  }
  memcpy(data, &qr, sizeof(sQrDet));
  return 0;
}

/**
 *
 */
alt_u32 qrThrSet(alt_u32 data)
{
  IOWR(QRCODE_FINDER_0_BASE, 1, data);
  return 0;
}
