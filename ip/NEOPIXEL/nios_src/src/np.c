/**
 *
 * @file np.c
 * @brief NeoPixel driver
 *
 */

#include "config.h"

#if defined(NP_MODULE) && (NP_MODULE == 1)

#include <string.h>
#include <stdlib.h>
#include <io.h>
#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
#include <altera_vic_irq.h>
#else
#include <sys/alt_irq.h>
#endif

#include "mb.h"
#include "gpio.h"
#include "tmr.h"
#include "gfx.h"
#include "np.h"

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

GFXgc     npGc;

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
alt_u32   npPin[] = {NP_PIN};     // list of used pins
alt_u32   npBufCur;               // pointer to current buffer
alt_u32   npBufNum;               // number of buffers
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

alt_u32   npStrIdx;               // string index, used in graphic

alt_u8    brightness[NP_DEV_NUM];
alt_u8    rOffset;       // Index of red byte within each 3- or 4-byte pixel
alt_u8    gOffset;       // Index of green byte
alt_u8    bOffset;       // Index of blue byte
alt_u8    wOffset;       // Index of white byte (same as rOffset if no white)

static void npTmrCb(void* arg);

/**
 */
void npInit(int channels)
{
  npBufCur = 0;
  npBufNum = channels;
  /* TODO move in initial set function */
  npZigZag = 1;
  npZzSts  = 0;
  npZzlLen = 8;
  npZzOfs  = npZzlLen * NP_DEV_NUM * sizeof(alt_u32);
}

/**
 */
void npCmd(void)
{
  alt_u32 volatile *rpc = (alt_u32*)MB_BASE;
  alt_u32 ret;

  ret = -1;
  switch(MB_CMD(rpc[0])){
  case 1:
    /* NeoPixel set global parameters */
    ret = npSet(rpc[1], rpc[2], rpc[3]);
    break;
  case 2:
    /* set single LED */
    ret = npLedSet(MB_SUB(rpc[0]), rpc[1], rpc[2], rpc[3], rpc[4], rpc[5]);
    break;
  case 3:
    /* set Brightness */
    ret = npBrgSet(MB_SUB(rpc[0]), rpc[1]);
    break;
  case 4:
    /* show actual configuration */
    ret = npLedShow();
    break;
  case 5:
    /* set timing */
    ret = npTmgSet(rpc[1], rpc[2], rpc[3], rpc[4], rpc[5]);
    break;
  case 6:
    /* set  buffers lenght, zigZag mode and zigzag line len */
    ret = npBufSet( rpc[2], rpc[3], rpc[4]);
    break;
  case 7:
    /* select work buffer */
    ret = npBufSel(rpc[1]);
    break;
  case 8:
    /* load, start and stop sequence */
    ret = npWrapSet(rpc[1], rpc[2], rpc[3]);
    break;
  case 9:
    /* load, start and stop sequence */
    ret = npSeqSet((psNpSeq)&rpc[1]);
    break;
  case 10:
    /* start and stop buffer loop */
    ret = npBufLoop(rpc[1], rpc[2], rpc[3]);
    break;
  case 11:
    /* Attach gfx to np buffer */
    ret = npGfxAtt(rpc[1]);
    break;
  case 12:
    ret = npGfxDet();
    break;
  case 13:
    /* set string where to write */
    ret = npGfxStrSet(rpc[1]);
    break;
  case 14:
    /* write pixel */
    ret = npGfxWp(rpc[1], rpc[2], rpc[3]);
    break;
  }
  rpc[1] = ret;
}

/**
 * Set number of work buffers
 */
alt_u32 npBufSet(alt_u32 len, alt_u32 zzf, alt_u32 zzl)
{
  npBufLen = len;
  npZigZag = zzf;
  npZzlLen = zzl;
  npZzSts  = 0;
  npZzOfs  = npZzlLen * NP_DEV_NUM * sizeof(alt_u32);
  return 0;
}

/**
 * Select work buffer
 */
alt_u32 npBufSel(alt_u32 idx)
{
  if (idx >= npBufNum) {
    return -1;
  }
  npBufCur = idx;
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
  return 0;
}

/**
 * Initial setup
 *
 * @param mask
 * @param numLEDs
 * @param type
 *
 * @return
 */
alt_u32 npSet(alt_u32 mask, alt_u16 numLEDs, alt_u32 type)
{
  alt_u32   bpp;                  // bit per pixel
  psNpTmg   tmg;
  int       i;

  if (npBufLen < numLEDs) {
    npBufLen = numLEDs;
  }

  npBufSize = NP_DEV_NUM * npBufLen * sizeof(alt_u32);
  if ((npBufSize * npBufNum) > NP_MEM_SIZE) {
    return -1;
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

  if (!numLEDs) {
    return 0;
  }
  memset((void*)NP_MEM_BASE, 0, npBufSize * npBufNum);

  npChMsk   = mask & NP_CH_MSK;
  npNumLed  = numLEDs;
  npBufCur  = 0;
  npWrpAdr  = 0;
  npWrpCnt  = numLEDs+1;
  npCtrlReg = NP_CTRL_START |
              (( npNumLed    << NP_CTRL_LEN_OFS) & NP_CTRL_LEN_MSK)|
              (( bpp         << NP_CTRL_CNT_OFS) & NP_CTRL_CNT_MSK);
  npWCntReg = (( npZigZag    << NP_WCNT_ZZM_OFS) & NP_WCNT_ZZM_MSK)|
              (( npZzSts     << NP_WCNT_ZZS_OFS) & NP_WCNT_ZZS_MSK)|
              (((npZzlLen-1) << NP_WCNT_LL_OFS ) & NP_WCNT_LL_MSK )|
              (( npWrpCnt    << NP_WCNT_WC_OFS ) & NP_WCNT_WC_MSK );

  for (i=0; i<NP_DEV_NUM; i++) {
    if (npChMsk & (1<<i)) {
      gpioPinMode(npPin[i], NP_PIN_MUX);
    }
  }

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
 * Set single LED
 */
alt_u32 npLedSet(alt_u32 idx, alt_u16 LED, alt_u8 r, alt_u8 g, alt_u8 b, alt_u8 w)
{
  if (idx >= NP_DEV_NUM ) {
    return -1;
  }
  if ((npChMsk && (1<<idx))==0) {
    return -1;
  }
  if (LED >= npNumLed) {
    return -1;
  }

  if (brightness[idx]) {
    r = (r * brightness[idx]) >> 8;
    g = (g * brightness[idx]) >> 8;
    b = (b * brightness[idx]) >> 8;
    w = (w * brightness[idx]) >> 8;
  }

  alt_u32 *pLed;

  pLed = (alt_u32*)NP_MEM_BASE;
  pLed += (npBufSize * npBufCur) >> 2;
  pLed += ((NP_DEV_NUM * LED) + idx);
  *pLed = ((alt_u32)r << rOffset) |
          ((alt_u32)g << gOffset) |
          ((alt_u32)b << bOffset) |
          ((alt_u32)w << wOffset) ;
  return 0;
}

/**
 */
alt_u32 npBrgSet(alt_u32 idx, alt_u8 brg)
{
  brightness[idx] = brg;
  return 0;
}

/**
 * Show
 */
alt_u32 npLedShow(void)
{
  /* check for idle state */
  if (IORD(NP_CSR_BASE, NP_REG_STATUS) != NP_STATUS_IDLE) {
    return -1;
  }
  alt_u32 base;

  /* set base address */
  base = (npBufSize * npBufCur) + npBufAdr;
/* TODO sembra non serva
  if ((npBufAdr % (8 * 2)) == 0) {
    base +=  8 * sizeof(alt_u32) * NP_DEV_NUM;
    npWCntReg |= ((1 << NP_WCNT_ZZS_OFS) & NP_WCNT_ZZS_MSK);
  } else {
    npWCntReg &= ~((1 << NP_WCNT_ZZS_OFS) & NP_WCNT_ZZS_MSK);
  }
*/
  IOWR(NP_CSR_BASE, NP_REG_START_ADR, NP_MEM_BASE + base);

/* TODO move in wrap set routine */
  /* set wrap count */
  npWCntReg &= ~NP_WCNT_WC_MSK;
  npWCntReg |= ((npWrpCnt  << NP_WCNT_WC_OFS ) & NP_WCNT_WC_MSK);
  IOWR(NP_CSR_BASE, NP_REG_WCNT, npWCntReg);

  base = (npBufSize * npBufCur) + npWrpAdr;
  /* set wrap address */
  IOWR(NP_CSR_BASE, NP_REG_WRAP_ADR, NP_MEM_BASE + base);
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
  npBufAdr = sAdr * npZzlLen * sizeof(alt_u32) * NP_DEV_NUM;
  /**
   * lunghezza va messa -1, il wrap viene confrontato dopo un giro
   * per cui va a -2
   */
  npWrpCnt = npNumLed - wCnt * npZzlLen + 2;
  if (wCnt & 1) {
    npWrpAdr = ((wAdr+1) * npZzlLen-1) * sizeof(alt_u32) * NP_DEV_NUM;
  } else {
    npWrpAdr = wAdr * npZzlLen * sizeof(alt_u32) * NP_DEV_NUM;
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
alt_u32 npBufLoop(alt_u32 flg, alt_u32 buf, alt_u32 ms)
{

  if (flg & NP_SEQ_FLG_STOP) {
    tmrStart(0, NULL, NULL);
  }else
  if (flg & NP_SEQ_FLG_START) {
    npSeq.flg = flg;
    if (npSeq.flg & NP_SEQ_FLG_INV_LOOP) {
      npSeqNum = (npBufLen / npZzlLen)- 1;
    } else {
      npSeqNum = 0;
    }
    tmrStart(ms, npTmrCb, (void*)ms);
  }
  return 0;
}

/**
 */
alt_u32 npGfxAtt(alt_u32 flg)
{
  npGc.width  = npZzlLen;
  npGc.height = npNumLed / npZzlLen;
  npGc.stride = NP_DEV_NUM;
  npGc.bpp    = 32;
  npGc.fmt    = GFX_GC_FMT_XGRB32;
  npGc.fb     = NP_MEM_BASE;
  npGc.wp     = npGfxWp;
  npGc.flg    = flg;
  npGc.cursor_x = 0;
  npGc.cursor_y = 0;

  gcSet(&npGc);

  npStrIdx = 0;

  return 0;
}

alt_u32 npGfxDet(void)
{

  gcSet(NULL);

  return 0;
}

/**
 */
alt_u32 npGfxStrSet(alt_u32 idx)
{
  npStrIdx = idx;
  return 0;
}

/**
 */
alt_u32 npGfxWp(alt_u32 x, alt_u32 y, alt_u32 color)
{
  alt_u16   led;
  alt_u32  *pLed;

  led = x + y * npZzlLen;
  pLed = npGc.fb;
  pLed += (npBufSize * npBufCur) >> 2;
  pLed += ((NP_DEV_NUM * led) + npStrIdx);
  *pLed = ((alt_u32)((color >> 16) & 0xFF) << rOffset) |
          ((alt_u32)((color >>  8) & 0xFF) << gOffset) |
          ((alt_u32)((color >>  0) & 0xFF) << bOffset) |
          ((alt_u32)((color >> 24) & 0xFF) << wOffset) ;
  return 0;
}

/**
 */
static void npTmrCb(void* arg)
{
  if (npSeq.flg & NP_SEQ_FLG_BUF_LOOP) {
    npWrapSet(npSeqNum, (npBufLen / npZzlLen) - npSeqNum, 0);
    npLedShow();
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
    npBufCur = npSeq.seq[npSeqNum].nBuf;
    npWrapSet(npSeq.seq[npSeqNum].sAdr, npSeq.seq[npSeqNum].wCnt, npSeq.seq[npSeqNum].wAdr);
    npLedShow();
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

#endif /* defined(NP_MODULE) && (NP_MODULE == 1) */
