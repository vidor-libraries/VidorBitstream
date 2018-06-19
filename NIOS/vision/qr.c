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
alt_u32 qrThrSet(alt_u32 data);

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
  //qrThrSet(120);
  qr.sts = QR_STS_NOQR;
  qrEnable = 1;
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
  }
  rpc[1] = ret;
}


/*
void qrCross(alt_u32 x, alt_u32 y, alt_u32 c)
{
   if (x<4) x=4;
   if (y<4) y=4;
   writeLine(x-4, y, x+4, y, c);
   writeLine(x, y-4, x, y+4, c);
}
*/
/**
 */
void qrLoop(void)
{
  alt_u32 temp, ty, txs, txe, tc, tmaxy;
  sQrPnt  pt[20];

  if (qrEnable) {
    int ctrl;
    int i, j;

    ctrl = IORD(QRCODE_FINDER_0_BASE, 0);
    if (ctrl & 1) {
      qr.sts = QR_STS_BUSY;
      tmaxy = 0;
      for(i=0; i<20; i++){
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
        for (j=0; j<20; j++) {
          if (pt[j].valid) {
            if ((tc >= pt[j].xs) && (tc <= pt[j].xe) && ((ty-pt[j].ye)<4)) {
              pt[j].ye = ty;
              if (tmaxy < (pt[j].ye-pt[j].ys)) {
                tmaxy = (pt[j].ye-pt[j].ys);
              }
              break;
            }
          } else {
            break;
          }
        }
        if (j<20 && !pt[j].valid) {
          pt[j].valid = 1;
          pt[j].xs = txs;
          pt[j].xe = txe;
          pt[j].ys = ty;
          pt[j].ye = ty;
        }
      }

      for (i=0; i<3; i++) {
        //if (qr.pt[i].valid==1) {
        //  qrCross((qr.pt[i].xe+qr.pt[i].xs)/2,(qr.pt[i].ye+qr.pt[i].ys)/2,0);
        //}
        qr.pt[i].valid = 0;
      }

      j = 0;
      for (i=0; i<20; i++) {
        if (pt[i].valid && (pt[i].ye-pt[i].ys)<(tmaxy/2)) {
          pt[i].valid = 0;
        } else if (pt[i].valid) {
          qr.pt[j].xs = pt[i].xs;
          qr.pt[j].xe = pt[i].xe;
          qr.pt[j].ys = pt[i].ys;
          qr.pt[j].ye = pt[i].ye;
          pt[i].valid = 0;
          qr.pt[j].valid = 1;
          //qrCross((qr.pt[j].xe+qr.pt[j].xs)/2, (qr.pt[j].ye+qr.pt[j].ys)/2, 0xffff);
          j++;
          if (j==3) {
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
alt_u32 qrGet(alt_u32* data)
{
  while(qr.sts == QR_STS_BUSY);
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
