/**
 * Copyright 2018 ARDUINO SA (http://www.arduino.cc/)
 * This file is part of Vidor IP.
 * Copyright (c) 2018
 * Authors: Massimiliano Agneni
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
 * @file np.c
 * @brief NeoPixel driver
 *
 */


#include <string.h>
#include <stdlib.h>
#include <io.h>

#include "config.h"
#include "mb.h"
#include "tmr.h"
#include "np.h"

#ifdef NP_GFX
#include "gfx.h"
#endif

#define NP_CSR_BASE NEOPIXEL_BASE
#define NP_MEM_BASE SDRAM_BASE
#define NP_MEM_SIZE SDRAM_SPAN

/**
 * NEOPIXEL CSR registers
 *
 * ofs
 *  1     15       [14:5]       [4:0]
 *    rSTART, rSTRING_LEN, rBIT_COUNT
 *  2 [15:0]
 *    rTRESET
 *  3 [15:0]
 *    rT0H
 *  4 [15:0]
 *    rT1H
 *  5 [15:0]
 *    rTT
 *  6      21        20       [19:10]        [9:0]
 *    rZIGZAG, rZZSTATE, rLINE_LENGTH, rWRAP_COUNT
 *  8 [31:0]
 *    rSTART_ADDRESS
 *  9 [31:0]
 *    rWRAP_ADDRESS
 * 10 [31:0]
 *    rZZ_OFFSET       jump in bytes
 *
 * Note
  *  Register 1
 *    Writing this register clears interrupt request.
 *    If start bit is set a string update cycle is started.
 *
 */
#define NP_REG_CH_MSK       0
#define NP_CH_MSK                 0x007FFFFF
#define NP_REG_CTRL         1
#define NP_CTRL_START             0x00008000
#define NP_CTRL_LEN_MSK           0x00007FE0
#define NP_CTRL_LEN_OFS           5
#define NP_CTRL_CNT_MSK           0x0000001F
#define NP_CTRL_CNT_OFS           0
#define NP_REG_TRESET       2
#define NP_REG_T0H          3
#define NP_REG_T1H          4
#define NP_REG_TTOT         5
#define NP_REG_WCNT         6
#define NP_WCNT_ZZM_MSK           0x00200000
#define NP_WCNT_ZZM_OFS           21
#define NP_WCNT_ZZS_MSK           0x00100000
#define NP_WCNT_ZZS_OFS           20
#define NP_WCNT_LL_MSK            0x000FFC00
#define NP_WCNT_LL_OFS            10
#define NP_WCNT_WC_MSK            0x000003FF
#define NP_WCNT_WC_OFS            0
#define NP_REG_STATUS       7
#define NP_STATUS_IDLE            0
#define NP_STATUS_PRELOAD         1
#define NP_STATUS_SHIFTING        2
#define NP_STATUS_RESETTING       3
#define NP_REG_START_ADR    8
#define NP_REG_WRAP_ADR     9
#define NP_REG_ZZ_OFS      10

#define CLOCK   (((float)ALT_CPU_FREQ)/1000000000.0)           // clock in GHz


#ifdef NP_GFX
GFXgc gfxNpGc[NEOPIXEL_CHANNELS];
#endif

typedef struct {
  alt_u32   trst;     // reset time
  alt_u32   t0h ;     // high time for 0
  alt_u32   t1h ;     // high time for 1
  alt_u32   ttot;     // total time for 1/0
}sNpTmg, *psNpTmg;

sNpTmg npTmg[] = {
  {
    (alt_u32)((float)300000.0*CLOCK),
    (alt_u32)((float)   480.0*CLOCK),
    (alt_u32)((float)  2000.0*CLOCK),
    (alt_u32)((float)  2480.0*CLOCK)
  },
  {
    (alt_u32)((float)300000.0*CLOCK),
    (alt_u32)((float)   350.0*CLOCK),
    (alt_u32)((float)   750.0*CLOCK),
    (alt_u32)((float)  1250.0*CLOCK)
  },
};

/**
 */
alt_u32   npBufLen;               // len of each buffer must be equal o greater of leds
alt_u32   npBufSize;              // size of each buffer
alt_u32   npBufAdr;               // start address
alt_u32   npWrpAdr;               // wrap address
alt_u32   npWrpCnt;               // counter to jump at wrap address
alt_u32   npNumLed;               // number of led per string
alt_u32   npCtrlReg;              // control register
alt_u32   npWCntReg;              // wrap count and zz control register
alt_u32   npChMsk;                // channel mask bitfield

alt_u32   npZzSts;
alt_u32   npZzOfs;
alt_u32   npZigZag;
alt_u32   npZzlLen;

sNpSeq    npSeq;
alt_u32   npSeqNum;               // sequence number

alt_u8    brightness[NEOPIXEL_CHANNELS];
alt_u8    rOffset;       // Index of red byte within each 3- or 4-byte pixel
alt_u8    gOffset;       // Index of green byte
alt_u8    bOffset;       // Index of blue byte
alt_u8    wOffset;       // Index of white byte (same as rOffset if no white)

static void npTmrCb(void* arg);

alt_u32 npSetup(alt_u32 cmd, alt_u32 led_num, alt_u32 type, alt_u32 buf_len, alt_u32 zzf, alt_u32 zzl);
alt_u32 npEnd(alt_u32 cmd);

alt_u32 npTmgSet(alt_u32 frq, alt_u32 trst, alt_u32 t0h, alt_u32 t1h, alt_u32 ttot);
alt_u32 npLedSet(alt_u32 cmd, alt_u16 LED, alt_u8 r, alt_u8 g, alt_u8 b, alt_u8 w);
alt_u32 npBrgSet(alt_u32 cmd, alt_u8 brg);
alt_u32 npLedShow(alt_u32 ofs);
alt_u32 npWrapSet(alt_u32 sAdr, alt_u32 wCnt, alt_u32 wAdr);
alt_u32 npSeqSet(psNpSeq pSeq);
alt_u32 npBufScroll(alt_u32 flg, alt_u32 buf, alt_u32 ms);

/**
 */
void npRpc(void)
{
  alt_u32 volatile *rpc = mbPtrGet();
  alt_u32 ret;

  ret = -1;
  if ((fpgaIp[RPC_GIID(rpc[0])].disc & 0xFFFFF) != NEOPIXEL_UID) {
    rpc[1] = ret;
    return ;
  }
  switch (RPC_PID(rpc[0])) {
  case 2:
    ret = npSetup(rpc[0], rpc[1], rpc[2], rpc[3], rpc[4], rpc[5]);
    break;
  case 4:
    ret = npEnd(rpc[0]);
    break;
  case 5:
    /* set timing */
    ret = npTmgSet(rpc[1], rpc[2], rpc[3], rpc[4], rpc[5]);
    break;
  case 6:
    /* set single LED */
    ret = npLedSet(rpc[0], rpc[1], rpc[2], rpc[3], rpc[4], rpc[5]);
    break;
  case 7:
    /* set Brightness */
    ret = npBrgSet(rpc[0], rpc[1]);
    break;
  case 8:
    /* show actual configuration */
    ret = npLedShow(rpc[1]);
    break;
  case 9:
    /* set buffer wrapping */
    ret = npWrapSet(rpc[1], rpc[2], rpc[3]);
    break;
  case 10:
    /* load, start and stop sequence */
    ret = npSeqSet((psNpSeq)&rpc[1]);
    break;
  case 11:
    /* start and stop buffer loop */
    ret = npBufScroll(rpc[1], rpc[2], rpc[3]);
    break;
  }
  rpc[1] = ret;
}

/**
 * Initial setup
 *
 * @param cmd
 * @param led_num
 * @param type
 * @param buf_num
 * @param buf_len
 * @param zzf
 * @param zzl
 *
 * @return
 */
alt_u32 npSetup(alt_u32 cmd, alt_u32 led_num, alt_u32 type, alt_u32 buf_len, alt_u32 zzf, alt_u32 zzl)
{
  alt_u8    giid = RPC_GIID(cmd);
  alt_u32   bpp;                    // bit per pixel
  psNpTmg   tmg;                    // timeing structure pointer
  int       i;

  if (!led_num) {
    return 0;
  }

  if ((NEOPIXEL_CHANNELS * buf_len * sizeof(alt_u32)) > NP_MEM_SIZE) {
    return -1;
  }

  npChMsk = 0;
  for (i=0; i<FPGA_PINS_NUM; i++){
    if (fpgaPin[i].giid == giid) {
      npChMsk |= 1 << fpgaPin[i].chn;
    }
  }

  npNumLed = led_num;
  npBufLen = (buf_len < led_num)? led_num: buf_len;
  npZigZag = zzf;
  npZzlLen = zzl;

  npZzOfs   = npZzlLen * NEOPIXEL_CHANNELS * sizeof(alt_u32);
  npWrpAdr  = 0;
  npWrpCnt  = led_num + 1;
  npBufSize = NEOPIXEL_CHANNELS * npBufLen * sizeof(alt_u32);

  for (i=0; i<NEOPIXEL_CHANNELS; i++) {
    brightness[i] = 0;
  }

  wOffset = ((type >> 6) & 0b11) << 3; // See notes in header file
  rOffset = ((type >> 4) & 0b11) << 3; // regarding R/G/B/W offsets
  gOffset = ((type >> 2) & 0b11) << 3;
  bOffset = ( type       & 0b11) << 3;

  if (wOffset == rOffset) {
    bpp = 24-1;
  }else{
    bpp = 32-1;
  }

  if (type & 0x0100) {
    tmg = &npTmg[0]; /* SPEED_400K */
  }else {
    tmg = &npTmg[1]; /* SPEED_800K */
  }

  memset((void*)NP_MEM_BASE, 0, npBufSize);

#ifdef NP_GFX
  GFXgc    *gc;
  for (i=0; i<NEOPIXEL_CHANNELS; i++) {
    gc = &gfxNpGc[i];
    gc->width    = npZzlLen;
    gc->height   = npNumLed / npZzlLen;
    gc->stride   = NEOPIXEL_CHANNELS;
    gc->bpp      = 32;
    gc->fmt      = GFX_GC_FMT_XGRB32;   // TODO
    gc->color    = 0;
    gc->fb       = (void*)(NP_MEM_BASE + (i*4));
    gc->pix      = wp32;
    gc->flg      = GFX_GC_ROT90;        // TODO
    gc->pFnt     = NULL;
    gc->txtColor = 0;
    gc->cursor_x = 0;
    gc->cursor_y = 0;
    gc->size     = 0;
  }
#endif
  npCtrlReg = NP_CTRL_START |
              (( npNumLed    << NP_CTRL_LEN_OFS) & NP_CTRL_LEN_MSK)|
              (( bpp         << NP_CTRL_CNT_OFS) & NP_CTRL_CNT_MSK);
  npWCntReg = (( npZigZag    << NP_WCNT_ZZM_OFS) & NP_WCNT_ZZM_MSK)|
              (( npZzSts     << NP_WCNT_ZZS_OFS) & NP_WCNT_ZZS_MSK)|
              (((npZzlLen-1) << NP_WCNT_LL_OFS ) & NP_WCNT_LL_MSK )|
              (( npWrpCnt    << NP_WCNT_WC_OFS ) & NP_WCNT_WC_MSK );

  while (IORD(NP_CSR_BASE, NP_REG_STATUS) != NP_STATUS_IDLE);
  IOWR(NP_CSR_BASE, NP_REG_CH_MSK  , npChMsk  );
  IOWR(NP_CSR_BASE, NP_REG_CTRL    , 0        );
  IOWR(NP_CSR_BASE, NP_REG_TRESET  , tmg->trst);
  IOWR(NP_CSR_BASE, NP_REG_T0H     , tmg->t0h );
  IOWR(NP_CSR_BASE, NP_REG_T1H     , tmg->t1h );
  IOWR(NP_CSR_BASE, NP_REG_TTOT    , tmg->ttot);
  IOWR(NP_CSR_BASE, NP_REG_WCNT    , npWCntReg);
  IOWR(NP_CSR_BASE, NP_REG_WRAP_ADR, npWrpAdr );
  IOWR(NP_CSR_BASE, NP_REG_ZZ_OFS  , npZzOfs  );

  return 0;
}

/**
 */
alt_u32 npEnd(alt_u32 cmd)
{
  return 0;
}

/**
 * Timing set
 */
alt_u32 npTmgSet(alt_u32 frq, alt_u32 trst, alt_u32 t0h, alt_u32 t1h, alt_u32 ttot)
{
  if (frq>1) {
    return -1;
  }
  npTmg[frq].trst = (alt_u32)((float)trst*CLOCK);
  npTmg[frq].t0h  = (alt_u32)((float)t0h *CLOCK);
  npTmg[frq].t1h  = (alt_u32)((float)t1h *CLOCK);
  npTmg[frq].ttot = (alt_u32)((float)ttot*CLOCK);

  while (IORD(NP_CSR_BASE, NP_REG_STATUS) != NP_STATUS_IDLE);
  IOWR(NP_CSR_BASE, NP_REG_TRESET  , npTmg[frq].trst);
  IOWR(NP_CSR_BASE, NP_REG_T0H     , npTmg[frq].t0h );
  IOWR(NP_CSR_BASE, NP_REG_T1H     , npTmg[frq].t1h );
  IOWR(NP_CSR_BASE, NP_REG_TTOT    , npTmg[frq].ttot);

  return 0;
}

/**
 * Set single LED
 */
alt_u32 npLedSet(alt_u32 cmd, alt_u16 LED, alt_u8 r, alt_u8 g, alt_u8 b, alt_u8 w)
{
  alt_u16 chn = RPC_CHN(cmd);

  if (chn >= NEOPIXEL_CHANNELS ) {
    return -1;
  }
  if ((npChMsk && (1<<chn))==0) {
    return -1;
  }
  if (LED >= npNumLed) {
    return -1;
  }

  if (brightness[chn]) {
    r = (r * brightness[chn]) >> 8;
    g = (g * brightness[chn]) >> 8;
    b = (b * brightness[chn]) >> 8;
    w = (w * brightness[chn]) >> 8;
  }

  alt_u32 *pLed;

  pLed = (alt_u32*)NP_MEM_BASE;
  pLed += ((NEOPIXEL_CHANNELS * LED) + chn);
  *pLed = ((alt_u32)r << rOffset) |
          ((alt_u32)g << gOffset) |
          ((alt_u32)b << bOffset) |
          ((alt_u32)w << wOffset) ;
  return 0;
}

/**
 */
alt_u32 npBrgSet(alt_u32 cmd, alt_u8 brg)
{
  alt_u16 chn = RPC_CHN(cmd);

  if (chn >= NEOPIXEL_CHANNELS ) {
//     return -1;
  }
  if ((npChMsk && (1<<chn))==0) {
    return -1;
  }
  brightness[chn] = brg;
  return 0;
}

/**
 * Show
 */
alt_u32 npLedShow(alt_u32 ofs)
{
  /* check for idle state */
  if (IORD(NP_CSR_BASE, NP_REG_STATUS) != NP_STATUS_IDLE) {
    return -1;
  }

  ofs = ofs * npZzlLen;

  /* set base address */
  IOWR(NP_CSR_BASE, NP_REG_START_ADR, NP_MEM_BASE + npBufAdr + ofs);

/* TODO move in wrap set routine */
  /* set wrap count */
  npWCntReg &= ~NP_WCNT_WC_MSK;
  npWCntReg |= ((npWrpCnt  << NP_WCNT_WC_OFS ) & NP_WCNT_WC_MSK);
  IOWR(NP_CSR_BASE, NP_REG_WCNT, npWCntReg);

  /* set wrap address */
  IOWR(NP_CSR_BASE, NP_REG_WRAP_ADR, NP_MEM_BASE + npWrpAdr + ofs);
/* TODO move in wrap set routine */

  /* start transfer */
  IOWR(NP_CSR_BASE, NP_REG_CTRL, npCtrlReg);

  /* wait for transfer to complete */
  while (IORD(NP_CSR_BASE, NP_REG_STATUS) != NP_STATUS_IDLE);

  return 0;
}

/**
 */
alt_u32 npWrapSet(alt_u32 sAdr, alt_u32 wCnt, alt_u32 wAdr)
{
  npBufAdr = sAdr * npZzlLen * sizeof(alt_u32) * NEOPIXEL_CHANNELS;
  /**
   * lunghezza va messa -1, il wrap viene confrontato dopo un giro
   * per cui va a -2
   */
  npWrpCnt = npNumLed - wCnt * npZzlLen + 2;
  if (wCnt & 1) {
    npWrpAdr = ((wAdr+1) * npZzlLen-1) * sizeof(alt_u32) * NEOPIXEL_CHANNELS;
  } else {
    npWrpAdr = wAdr * npZzlLen * sizeof(alt_u32) * NEOPIXEL_CHANNELS;
  }
  return 0;
}

/**
 */
alt_u32 npSeqSet(psNpSeq pSeq)
{
  if (pSeq->num > 64) {
    return -1;
  }
  memcpy(&npSeq, pSeq, 8 + pSeq->num * 12);

  if (npSeq.flg & NP_SEQ_FLG_STOP) {
    tmrStart(0, NULL, NULL);
  }else
  if (npSeq.flg & NP_SEQ_FLG_START) {
    npSeqNum = 0;
    tmrStart(npSeq.seq[npSeqNum].ms, npTmrCb, NULL);
  }
  return 0;
}

/**
 */
alt_u32 npBufScroll(alt_u32 flg, alt_u32 buf, alt_u32 ms)
{

  if (flg & NP_SEQ_FLG_STOP) {
    tmrStart(0, NULL, NULL);
  }else
  if (flg & NP_SEQ_FLG_START) {
    npSeq.flg = flg;
    if (npSeq.flg & NP_SEQ_FLG_INV_LOOP) {
      npSeqNum = (npBufLen / npZzlLen) - 1;
    } else {
      npSeqNum = 0;
    }
    tmrStart(ms, npTmrCb, (void*)ms);
  }
  return 0;
}

/**
 */
static void npTmrCb(void* arg)
{
  if (npSeq.flg & NP_SEQ_FLG_BUF_LOOP) {
/**/
    if ((npSeqNum + npNumLed / npZzlLen) > (npBufLen / npZzlLen)) {
      npWrapSet(npSeqNum, (npBufLen / npZzlLen) - npSeqNum, 0);
    } else {
      npWrapSet(npSeqNum, npNumLed / npZzlLen, 0);
    }
/**/
//    npWrapSet(npSeqNum, (npBufLen / npZzlLen) - npSeqNum, 0);
    npLedShow(0);
/**/
    if (npSeq.flg & NP_SEQ_FLG_INV_LOOP) {
      if (npSeqNum == 0) {
        npSeqNum = npBufLen / npZzlLen;
      }
      npSeqNum--;
    } else {
      npSeqNum++;
      if (npSeqNum >= npBufLen / npZzlLen) {
        npSeqNum = 0;
      }
    }
    tmrStart((alt_u32)arg, npTmrCb, arg);
  } else {
    npWrapSet(npSeq.seq[npSeqNum].sAdr, npSeq.seq[npSeqNum].wCnt, npSeq.seq[npSeqNum].wAdr);
    npLedShow(npSeq.seq[npSeqNum].ofs);
    npSeqNum++;
    if (npSeqNum >= npSeq.num) {
      if (npSeq.flg & NP_SEQ_FLG_LOOP) {
        npSeqNum = 0;
      }else
      if (npSeq.flg & NP_SEQ_FLG_SINGLE) {
        return;
      }
    }
    tmrStart(npSeq.seq[npSeqNum].ms, npTmrCb, NULL);
  }
}
