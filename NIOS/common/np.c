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

/**
 * NEOPIXEL CSR registers
 */
#define NP_REG_CH_MSK   0
#define NP_CH_MSK             0x007FFFFF
#define NP_REG_CTRL     1
#define NP_CTRL_START         0x00008000
#define NP_CTRL_LEN_MSK       0x00007FE0
#define NP_CTRL_LEN_OFS       5
#define NP_CTRL_CNT_MSK       0x0000001F
#define NP_CTRL_CNT_OFS       0
#define NP_REG_TRESET   2
#define NP_REG_T0H      3
#define NP_REG_T1H      4
#define NP_REG_TTOT     5
#define NP_REG_STATUS   7
#define NP_STATUS_IDLE        0
#define NP_STATUS_PRELOAD     1
#define NP_STATUS_SHIFTING    2
#define NP_STATUS_RESETTING   3

#define CLOCK   (((float)ALT_CPU_FREQ)/1000000000.0)           // clock in GHz

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
alt_u32 npPin[] = {NP_PIN};     // list of used pins
alt_u32 npCtrlReg;
alt_u32 npChMsk;                // channel mask bitfield
alt_u32 npNumLed;

alt_u8  brightness[sizeof(npPin)/sizeof(alt_u32)];
alt_u8  rOffset;       // Index of red byte within each 3- or 4-byte pixel
alt_u8  gOffset;       // Index of green byte
alt_u8  bOffset;       // Index of blue byte
alt_u8  wOffset;       // Index of white byte (same as rOffset if no white)

/**
 */
alt_u32 npTmgSet(alt_u32 frq, alt_u32 trst, alt_u32 t0h, alt_u32 t1h, alt_u32 ttot);
alt_u32 npSet(alt_u32 mask, alt_u16 numLEDs, alt_u32 type);
alt_u32 npLedSet(alt_u32 idx, alt_u16 LED, alt_u8 r, alt_u8 g, alt_u8 b, alt_u8 w);
alt_u32 npBrgSet(alt_u32 idx, alt_u8 brg);
alt_u32 npLedShow(void);

/**
 */
void npInit(int devs)
{
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
  }
  rpc[1] = ret;
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
 */
alt_u32 npSet(alt_u32 mask, alt_u16 numLEDs, alt_u32 type)
{
  alt_u32   bpp;                  // bit per pixel
  /*
  alt_u32   treset;               // reset time
  alt_u32   t0h;                  // high time for 0
  alt_u32   t1h;                  // high time for 1
  alt_u32   ttot;                 // total time for 1/0
  */
  psNpTmg   tmg;
  int       i;

#if 0
  for (i=0; i<NP_DEV_NUM; i++) {
    gpioPinMode(npPin[i], 0);
  }
#endif

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
    /* SPEED_400K */
    tmg = &npTmg[0];
/*
    trst = (alt_u32)((float)300000.0*CLOCK);
    t0h  = (alt_u32)((float)   480.0*CLOCK);
    t1h  = (alt_u32)((float)  2000.0*CLOCK);
    ttot = (alt_u32)((float)  2480.0*CLOCK);
*/
  }else {
    /* SPEED_800K */
    tmg = &npTmg[1];
/*
    trst = (alt_u32)((float)300000.0*CLOCK);
    t0h  = (alt_u32)((float)   350.0*CLOCK);
    t1h  = (alt_u32)((float)   750.0*CLOCK);
    ttot = (alt_u32)((float)  1250.0*CLOCK);
*/
  }

  if (!numLEDs) {
    return 0;
  }
  memset((void*)NP_MEM_BASE, 0, NP_DEV_NUM*numLEDs*sizeof(alt_u32));

  npChMsk   = mask & NP_CH_MSK;
  npNumLed  = numLEDs;
  npCtrlReg = NP_CTRL_START |
              ((npNumLed << NP_CTRL_LEN_OFS) & NP_CTRL_LEN_MSK) |
              bpp;

  for (i=0; i<NP_DEV_NUM; i++) {
    if (npChMsk & (1<<i)) {
      gpioPinMode(npPin[i], NP_PIN_MUX);
    }
  }

  while (IORD(NP_CSR_BASE, NP_REG_STATUS) != NP_STATUS_IDLE);
  IOWR(NP_CSR_BASE, NP_REG_CH_MSK, npChMsk);
  IOWR(NP_CSR_BASE, NP_REG_CTRL  , 0);
  IOWR(NP_CSR_BASE, NP_REG_TRESET, tmg->trst);
  IOWR(NP_CSR_BASE, NP_REG_T0H   , tmg->t0h );
  IOWR(NP_CSR_BASE, NP_REG_T1H   , tmg->t1h );
  IOWR(NP_CSR_BASE, NP_REG_TTOT  , tmg->ttot);

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

  if (brightness[idx]) { // See notes in setBrightness()
    r = (r * brightness[idx]) >> 8;
    g = (g * brightness[idx]) >> 8;
    b = (b * brightness[idx]) >> 8;
    w = (w * brightness[idx]) >> 8;
  }

  alt_u32 *pLed;

  pLed = (alt_u32*)NP_MEM_BASE;
  pLed += ((NP_DEV_NUM*LED) + idx);
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

  /* start transfer */
  IOWR(NP_CSR_BASE, NP_REG_CTRL, npCtrlReg);

  /* wait for transfer complete */
  while (IORD(NP_CSR_BASE, NP_REG_STATUS) != NP_STATUS_IDLE);

  return 0;
}

#endif /* defined(NP_MODULE) && (NP_MODULE == 1) */
