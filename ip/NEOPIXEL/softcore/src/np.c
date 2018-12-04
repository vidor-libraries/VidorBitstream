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

#include "mb.h"
#include "pio.h"
#include "np.h"

#if defined(NP_USE_TMR) && (NP_USE_TMR == 1)
  #include "tmr.h"
#endif

#if defined(NP_GFX) && (NP_GFX == 1)
  #include "gfx.h"
  GFXgc npGfxGc[NEOPIXEL_0_CHANNELS];
#endif

/**
 * NEOPIXEL CSR registers
 *
 * ofs
 *  0 31-CHANNELS (CHANNELS-1)-0
 *                channels mask
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
#define NP_CTRL_APA               0x00200000
#define NP_CTRL_START             0x00100000
#define NP_CTRL_LEN_MSK           0x000FFFE0
#define NP_CTRL_LEN_OFS           5
#define NP_CTRL_CNT_MSK           0x0000001F
#define NP_CTRL_CNT_OFS           0
#define NP_REG_TRESET       2
#define NP_REG_T0H          3
#define NP_REG_T1H          4
#define NP_REG_TTOT         5
#define NP_REG_WCNT         6
#define NP_WCNT_ZZM_MSK           0x80000000
#define NP_WCNT_ZZM_OFS           31
#define NP_WCNT_ZZS_MSK           0x40000000
#define NP_WCNT_ZZS_OFS           30
#define NP_WCNT_LL_MSK            0x3FFF8000
#define NP_WCNT_LL_OFS            15
#define NP_WCNT_WC_MSK            0x00007FFF
#define NP_WCNT_WC_OFS            0
#define NP_REG_STATUS       7
#define NP_STATUS_IDLE            0
#define NP_STATUS_PRELOAD         1
#define NP_STATUS_SHIFTING        2
#define NP_STATUS_RESETTING       3
#define NP_REG_START_ADR    8
#define NP_REG_WRAP_ADR     9
#define NP_REG_ZZ_OFS      10

/**
 */
typedef struct {
  alt_u32   trst;     // reset time
  alt_u32   t0h ;     // high time for 0
  alt_u32   t1h ;     // high time for 1
  alt_u32   ttot;     // total time for 1/0
}sNpTmg, *psNpTmg;

/**
 */
typedef struct {
  alt_u32   flg;
  alt_u32   num;
  struct {
    alt_u32   ms;
    alt_u16   ofs;
    alt_u16   sAdr;
    alt_u16   wCnt;
    alt_u16   wAdr;
  }         seq[64];
}sNpSeq, *psNpSeq;

#define NP_SEQ_FLG_START      0x00000001
#define NP_SEQ_FLG_STOP       0x00000002
#define NP_SEQ_FLG_SINGLE     0x00000004
#define NP_SEQ_FLG_LOOP       0x00000008
#define NP_SEQ_FLG_BUF_LOOP   0x00000010
#define NP_SEQ_FLG_INV_LOOP   0x00000020

/**
 */
sNpTmg npTmg[] = {
  {
    (alt_u32)((float)300000.0*NP_CLOCK),
    (alt_u32)((float)   480.0*NP_CLOCK),
    (alt_u32)((float)  2000.0*NP_CLOCK),
    (alt_u32)((float)  2480.0*NP_CLOCK)
  },
  {
    (alt_u32)((float)300000.0*NP_CLOCK),
    (alt_u32)((float)   350.0*NP_CLOCK),
    (alt_u32)((float)   750.0*NP_CLOCK),
    (alt_u32)((float)  1250.0*NP_CLOCK)
  },
};

/**
 */
alt_u32   npMemBase;
alt_u32   npMemSize;
alt_u32   npCmd;
alt_u32   npBufLen;               // len of each buffer must be equal o greater of leds
alt_u32   npBufSize;              // size of each buffer
alt_u32   npBufAdr;               // start address
alt_u32   npWrpAdr;               // wrap address
alt_u32   npWrpCnt;               // counter to jump at wrap address
alt_u32   npNumLed;               // number of led per string
alt_u32   npBpp;                  // number of bit per pixel
alt_u32   npCtrlReg;              // control register
alt_u32   npWCntReg;              // wrap count and zz control register
alt_u32   npChMsk;                // channel mask bitfield

alt_u32   npZzSts;
alt_u32   npZzOfs;
alt_u32   npZigZag;
alt_u32   npZzlLen;

sNpSeq    npSeq;
alt_u32   npSeqNum;               // sequence number

alt_u8    brightness[NEOPIXEL_0_CHANNELS];
alt_u8    rOffset;       // Index of red byte
alt_u8    gOffset;       // Index of green byte
alt_u8    bOffset;       // Index of blue byte
alt_u8    wOffset;       // Index of white byte (same as rOffset if no white)

alt_u32 npSetup(alt_u32 cmd, alt_u32 led_num, alt_u32 type, alt_u32 buf_len, alt_u32 zzf, alt_u32 zzl, alt_u32 flg);
alt_u32 npEnd(alt_u32 cmd);

alt_u32 npTmgSet(alt_u32 cmd, alt_u32 frq, alt_u32 trst, alt_u32 t0h, alt_u32 t1h, alt_u32 ttot);
alt_u32 npLedSet(alt_u32 cmd, alt_u16 LED, alt_u8 r, alt_u8 g, alt_u8 b, alt_u8 w);
alt_u32 npBrgSet(alt_u32 cmd, alt_u8 brg);
alt_u32 npLedShow(alt_u32 cmd, alt_u32 ofs);
alt_u32 npWrapSet(alt_u32 cmd, alt_u32 sAdr, alt_u32 wCnt, alt_u32 wAdr);

#if defined(NP_USE_TMR) && (NP_USE_TMR == 1)
static void npTmrCb(void* arg);
alt_u32 npSeqSet(alt_u32 cmd, psNpSeq pSeq);
alt_u32 npBufScroll(alt_u32 cmd, alt_u32 flg, alt_u32 buf, alt_u32 ms);
#endif /* defined(NP_USE_TMR) && (NP_USE_TMR == 1) */

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
    ret = npSetup(rpc[0], rpc[1], rpc[2], rpc[3], rpc[4], rpc[5], rpc[6]);
    break;
  case 4:
    ret = npEnd(rpc[0]);
    break;
  case 5:
    /* set timing */
    ret = npTmgSet(rpc[0], rpc[1], rpc[2], rpc[3], rpc[4], rpc[5]);
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
    ret = npLedShow(rpc[0], rpc[1]);
    break;
  case 9:
    /* set buffer wrapping */
    ret = npWrapSet(rpc[0], rpc[1], rpc[2], rpc[3]);
    break;
#if defined(NP_USE_TMR) && (NP_USE_TMR == 1)
  case 10:
    /* load, start and stop sequence */
    ret = npSeqSet(rpc[0], (psNpSeq)&rpc[1]);
    break;
  case 11:
    /* start and stop buffer loop */
    ret = npBufScroll(rpc[0], rpc[1], rpc[2], rpc[3]);
    break;
#endif  /* defined(NP_USE_TMR) && (NP_USE_TMR == 1) */
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
alt_u32 npSetup(alt_u32 cmd, alt_u32 led_num, alt_u32 type, alt_u32 buf_len,
                alt_u32 zzf, alt_u32 zzl, alt_u32 flg)
{
  alt_u8    giid = RPC_GIID(cmd);
  psNpTmg   tmg;                    // timing structure pointer
  int       i;
  alt_u32   base = fpgaIp[giid].base;
  alt_u32   chns = RPC_CHN_GET(fpgaIp[giid].disc);

  if (!led_num) {
    return 0;
  }

  npMemBase = ((psNpPriv)(fpgaIp[giid].priv))->mem_base;
  npMemSize = ((psNpPriv)(fpgaIp[giid].priv))->mem_size;

  buf_len = (buf_len < led_num)? led_num: buf_len;
  if ((chns * buf_len * sizeof(alt_u32)) > npMemSize) {
    return -1;
  }

  npChMsk = 0;
  for (i=0; i<FPGA_PINS_NUM; i++){
    if (fpgaPin[i].giid == giid) {
      npChMsk |= 1 << fpgaPin[i].chn;
    }
  }

  npNumLed = led_num;
  npBufLen = buf_len;
  npZigZag = zzf;
  npZzlLen = zzl;

  npZzOfs   = npZzlLen * chns * sizeof(alt_u32);
  npWrpAdr  = 0;
  npWrpCnt  = led_num + 1;
  npBufSize = chns * npBufLen * sizeof(alt_u32);

  for (i=0; i<chns; i++) {
    brightness[i] = 0;
  }

  wOffset = ((type >> 6) & 0b11) << 3;
  rOffset = ((type >> 4) & 0b11) << 3;
  gOffset = ((type >> 2) & 0b11) << 3;
  bOffset = ( type       & 0b11) << 3;

  if (wOffset == rOffset) {
    npBpp = 24;
  }else{
    npBpp = 32;
  }

  if (type & 0x0100) {
    tmg = &npTmg[0]; /* SPEED_400K */
  }else {
    tmg = &npTmg[1]; /* SPEED_800K */
  }

  memset((void*)npMemBase, 0, npBufSize);

#if defined(NP_GFX) && (NP_GFX == 1)
  for (i=0; i<chns; i++) {
    npGfxGc[i].width    = npZzlLen;
    npGfxGc[i].height   = npNumLed / npZzlLen;
    npGfxGc[i].stride   = chns;
    npGfxGc[i].bpp      = 32;
    npGfxGc[i].fmt      = GFX_GC_FMT_XGRB32;   // TODO
    npGfxGc[i].color    = 0;
    npGfxGc[i].fb       = (void*)(npMemBase + (i*4));
    npGfxGc[i].pix      = wp32;
    npGfxGc[i].rdp      = rd32;
    npGfxGc[i].flg      = flg;
#if defined(GFX_FONTS) && (GFX_FONTS == 1)
    npGfxGc[i].pFnt     = NULL;
    npGfxGc[i].txtColor = 0;
    npGfxGc[i].cursor_x = 0;
    npGfxGc[i].cursor_y = 0;
    npGfxGc[i].size     = 0;
#endif /* defined(GFX_FONTS) && (GFX_FONTS == 1) */
  }
#endif
  npCtrlReg = NP_CTRL_START |
              (( npNumLed    << NP_CTRL_LEN_OFS) & NP_CTRL_LEN_MSK)|
              (((npBpp - 1)  << NP_CTRL_CNT_OFS) & NP_CTRL_CNT_MSK);
  npWCntReg = (( npZigZag    << NP_WCNT_ZZM_OFS) & NP_WCNT_ZZM_MSK)|
              (( npZzSts     << NP_WCNT_ZZS_OFS) & NP_WCNT_ZZS_MSK)|
              (((npZzlLen-1) << NP_WCNT_LL_OFS ) & NP_WCNT_LL_MSK )|
              (( npWrpCnt    << NP_WCNT_WC_OFS ) & NP_WCNT_WC_MSK );

  if (type & 0xFFFF0000) {
    // configuring AREF pin MUX on CLOK
    pioMode(0, 0, PIN_MUX(fpgaIp[giid].chn[0].pin[0].pin),1, 0);

    // configuring IP for APA mode
    npCtrlReg |= NP_CTRL_APA;

    // configuring IP for 32 bit
    npCtrlReg = (npCtrlReg & ~NP_CTRL_CNT_MSK) | ((32-1) << NP_CTRL_CNT_OFS);
  }

  while (IORD(base, NP_REG_STATUS) != NP_STATUS_IDLE);
  IOWR(base, NP_REG_CH_MSK  , npChMsk  );
  IOWR(base, NP_REG_CTRL    , 0        );
  IOWR(base, NP_REG_TRESET  , tmg->trst);
  IOWR(base, NP_REG_T0H     , tmg->t0h );
  IOWR(base, NP_REG_T1H     , tmg->t1h );
  IOWR(base, NP_REG_TTOT    , tmg->ttot);
  IOWR(base, NP_REG_WCNT    , npWCntReg);
  IOWR(base, NP_REG_WRAP_ADR, npWrpAdr );
  IOWR(base, NP_REG_ZZ_OFS  , npZzOfs  );

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
alt_u32 npTmgSet(alt_u32 cmd, alt_u32 frq, alt_u32 trst, alt_u32 t0h, alt_u32 t1h, alt_u32 ttot)
{
  alt_u8    giid = RPC_GIID(cmd);
  alt_u32   base = fpgaIp[giid].base;

  if (frq>1) {
    return -1;
  }
  npTmg[frq].trst = (alt_u32)((float)trst*NP_CLOCK);
  npTmg[frq].t0h  = (alt_u32)((float)t0h *NP_CLOCK);
  npTmg[frq].t1h  = (alt_u32)((float)t1h *NP_CLOCK);
  npTmg[frq].ttot = (alt_u32)((float)ttot*NP_CLOCK);

  while (IORD(base, NP_REG_STATUS) != NP_STATUS_IDLE);
  IOWR(base, NP_REG_TRESET  , npTmg[frq].trst);
  IOWR(base, NP_REG_T0H     , npTmg[frq].t0h );
  IOWR(base, NP_REG_T1H     , npTmg[frq].t1h );
  IOWR(base, NP_REG_TTOT    , npTmg[frq].ttot);

  return 0;
}

/**
 * Set single LED
 */
alt_u32 npLedSet(alt_u32 cmd, alt_u16 LED, alt_u8 r, alt_u8 g, alt_u8 b, alt_u8 w)
{
  alt_u8  giid = RPC_GIID(cmd);
  alt_u32 chns = RPC_CHN_GET(fpgaIp[giid].disc);
  alt_u16 chn  = RPC_CHN(cmd);

  if (chn >= chns ) {
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

  pLed = (alt_u32*)npMemBase;
  pLed += ((chns * LED) + chn);
  if (npBpp == 32) {
    *pLed = ((alt_u32)r << rOffset) |
            ((alt_u32)g << gOffset) |
            ((alt_u32)b << bOffset) |
            ((alt_u32)w << wOffset) ;
  } else {
    *pLed = ((alt_u32)r << rOffset) |
            ((alt_u32)g << gOffset) |
            ((alt_u32)b << bOffset) ;
  }
  return 0;
}

/**
 */
alt_u32 npBrgSet(alt_u32 cmd, alt_u8 brg)
{
  alt_u8  giid = RPC_GIID(cmd);
  alt_u32 chns = RPC_CHN_GET(fpgaIp[giid].disc);
  alt_u16 chn = RPC_CHN(cmd);

  if (chn >= chns ) {
    return -1;
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
alt_u32 npLedShow(alt_u32 cmd, alt_u32 ofs)
{
  alt_u8    giid = RPC_GIID(cmd);
  alt_u32   base = fpgaIp[giid].base;

  /* check for idle state */
  if (IORD(base, NP_REG_STATUS) != NP_STATUS_IDLE) {
    return -1;
  }

  ofs = ofs * npZzlLen;

  /* set base address */
  IOWR(base, NP_REG_START_ADR, npMemBase + npBufAdr + ofs);

/* TODO move in wrap set routine */
  /* set wrap count */
  npWCntReg &= ~NP_WCNT_WC_MSK;
  npWCntReg |= ((npWrpCnt  << NP_WCNT_WC_OFS ) & NP_WCNT_WC_MSK);
  IOWR(base, NP_REG_WCNT, npWCntReg);

  /* set wrap address */
  IOWR(base, NP_REG_WRAP_ADR, npMemBase + npWrpAdr + ofs);
/* TODO move in wrap set routine */

  /* start transfer */
  IOWR(base, NP_REG_CTRL, npCtrlReg);

  /* wait for transfer to complete */
  while (IORD(base, NP_REG_STATUS) != NP_STATUS_IDLE);

  return 0;
}

/**
 */
alt_u32 npWrapSet(alt_u32 cmd, alt_u32 sAdr, alt_u32 wCnt, alt_u32 wAdr)
{
  alt_u8    giid = RPC_GIID(cmd);
  alt_u32   chns = RPC_CHN_GET(fpgaIp[giid].disc);

  npBufAdr = sAdr * npZzlLen * sizeof(alt_u32) * chns;
  /**
   * lenght is -1,
   * wrap counter is compared after a lap
   * so value is -2
   */
  npWrpCnt = npNumLed - wCnt * npZzlLen + 2;
  if (wCnt & 1) {
    npWrpAdr = ((wAdr+1) * npZzlLen-1) * sizeof(alt_u32) * chns;
  } else {
    npWrpAdr = wAdr * npZzlLen * sizeof(alt_u32) * chns;
  }
  return 0;
}

#if defined(NP_USE_TMR) && (NP_USE_TMR == 1)
/**
 */
alt_u32 npSeqSet(alt_u32 cmd, psNpSeq pSeq)
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
    npCmd = cmd;
    tmrStart(npSeq.seq[npSeqNum].ms, npTmrCb, NULL);
  }
  return 0;
}

/**
 */
alt_u32 npBufScroll(alt_u32 cmd, alt_u32 flg, alt_u32 buf, alt_u32 ms)
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
    npCmd = cmd;
    tmrStart(ms, npTmrCb, (void*)ms);
  }
  return 0;
}

/**
 */
static void npTmrCb(void* arg)
{
  if (npSeq.flg & NP_SEQ_FLG_BUF_LOOP) {
    if ((npSeqNum + npNumLed / npZzlLen) > (npBufLen / npZzlLen)) {
      npWrapSet(npCmd, npSeqNum, (npBufLen / npZzlLen) - npSeqNum, 0);
    } else {
      npWrapSet(npCmd, npSeqNum, npNumLed / npZzlLen, 0);
    }
    npLedShow(npCmd, 0);
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
    npWrapSet(npCmd, npSeq.seq[npSeqNum].sAdr, npSeq.seq[npSeqNum].wCnt, npSeq.seq[npSeqNum].wAdr);
    npLedShow(npCmd, npSeq.seq[npSeqNum].ofs);
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
#endif  /* defined(NP_USE_TMR) && (NP_USE_TMR == 1) */
