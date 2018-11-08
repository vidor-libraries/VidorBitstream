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
#include "qr.h"
#if defined(QR_USE_GFX) && (QR_USE_GFX == 1)
  #include "gfx.h"
#endif

#define QR_STS_BUSY  1
#define QR_STS_READY 2
#define QR_STS_NOQR  3

#define SEC_RAM __attribute__((__section__(".rwdata")))

/**
 *
 */
typedef struct {
  alt_u32  xs;
  alt_u32  xe;
  alt_u32  ys;
  alt_u32  ye;
  alt_u32  valid;
}sQrPnt, *psQrPnt;

/**
 */
typedef struct{
  alt_u32 sts;
  sQrPnt  pt[QR_PT_DET_NUM];
}sQrDet, *psQrDet;

alt_u32 qrBase;
int     qrEnable;
int     qrDraw;
sQrDet  qr;
alt_u32 qrCnt;

alt_u32 qrSetup(alt_u32 cmd);
alt_u32 qrEnd(alt_u32 cmd);

alt_u32 qrMode(alt_u32 mode);
alt_u32 qrGet(alt_u32* data);
alt_u32 qrThrSet(alt_u32 data);

/**
 */
void SEC_RAM qrRpc(void)
{
  alt_u32 volatile *rpc = mbPtrGet();
  alt_u32           ret;

  ret = -1;
  if ((fpgaIp[RPC_GIID(rpc[0])].disc & 0xFFFFF) != QR_UID) {
    rpc[1] = ret;
    return ;
  }
  switch (RPC_PID(rpc[0])) {
  case 2:
    ret = qrSetup(rpc[0]);
    break;
  case 4:
    ret = qrEnd(rpc[0]);
    break;
  case 5:
    /* enable QR Code detection */
    qrEnable = rpc[1];
    ret = 0;
    break;
  case 6:
    /* set display mode */
    ret = qrMode(rpc[1]);
    break;
  case 7:
    /* get qr detect */
    ret = qrGet((alt_u32*)&rpc[2]);
    break;
  case 8:
    /* set qr threshold */
    ret = qrThrSet(rpc[1]);
    break;
  case 9:
    /* set cross drawing */
    qrDraw = rpc[1];
    ret = 0;
    break;
  }
  rpc[1] = ret;
}

/**
 */
alt_u32 qrSetup(alt_u32 cmd)
{
  qrEnable = 0;
  qrDraw = 0;

  //qrThrSet(120);
  qr.sts = QR_STS_NOQR;
  qrCnt = 0;
  qrBase = (alt_u32)fpgaIp[RPC_GIID(cmd)].base;
  return 0;
}

/**
 */
alt_u32 qrEnd(alt_u32 cmd)
{
  return 0;
}

#if defined(QR_USE_GFX) && (QR_USE_GFX == 1)
/**
 */
void qrCross(alt_u32 x, alt_u32 y, alt_u32 c)
{
   if (x<4) x=4;
   if (y<4) y=4;
   writeLine(pGfxGc[0], x-4, y, x+4, y, c);
   writeLine(pGfxGc[0], x, y-4, x, y+4, c);
}
#endif

/**
 */
void SEC_RAM qrLoop(void)
{
  if (!qrBase) {
    return;
  }
  if (qrEnable) {
    alt_u32 temp, ty, txs, txe, tc, tmaxy;
    sQrPnt  pt[QR_PT_NUM];
    int ctrl;
    int i, j;
    int gc;

    ctrl = IORD(qrBase, 0);
    if (ctrl & 1) {
      qr.sts = QR_STS_BUSY;
      tmaxy = 0;
      for(i=0; i<QR_PT_NUM; i++){
        pt[i].valid = 0;
      }
      for(i=0; i<1024; i++){
        temp = IORD(qrBase, 1024+i);
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
        else if (j==QR_PT_NUM && gc!=-1) {
          pt[gc].valid = 1;
          pt[gc].xs = txs;
          pt[gc].xe = txe;
          pt[gc].ys = ty;
          pt[gc].ye = ty;
        }
      }

      for (i=0; i<QR_PT_DET_NUM; i++) {
#if defined(QR_USE_GFX) && (QR_USE_GFX == 1)
        if (qrDraw) {
          if (qr.pt[i].valid==1) {
            qrCross((qr.pt[i].xe+qr.pt[i].xs)/2, (qr.pt[i].ye+qr.pt[i].ys)/2, 0);
          }
        }
#endif
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
#if defined(QR_USE_GFX) && (QR_USE_GFX == 1)
          if (qrDraw) {
            qrCross((qr.pt[j].xe+qr.pt[j].xs)/2, (qr.pt[j].ye+qr.pt[j].ys)/2, 0xffffffff);
          }
#endif
          j++;
          if (j==QR_PT_DET_NUM) {
            break;
          }
        }
      }
      qr.sts = QR_STS_READY;
      qrCnt = 0;
      IOWR(qrBase, 0, ctrl);
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

  ctrl = IORD(qrBase, 0);
  ctrl &= ~0x00000006;
  switch (mode) {
  case 0: ctrl |= 0x00000000; break;
  case 1: ctrl |= 0x00000002; break;
  case 2: ctrl |= 0x00000004; break;
  case 3: ctrl |= 0x00000006; break;
  }
  IOWR(qrBase, 0, ctrl);

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
  int       i;
  alt_u32  *ptr = (alt_u32*)&qr;
  for (i=0; i<sizeof(sQrDet)/sizeof(alt_u32); i++) {
    data[i] = ptr[i];
  }
  //memcpy(data, &qr, sizeof(sQrDet));
  return 0;
}

/**
 *
 */
alt_u32 qrThrSet(alt_u32 data)
{
  IOWR(qrBase, 1, data);
  return 0;
}
