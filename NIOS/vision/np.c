/**
 *
 * @file np.c
 * @brief NeoPixel driver
 *
 */

#include <string.h>
#include <stdlib.h>
#include <io.h>
#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
#include <altera_vic_irq.h>
#else
#include <sys/alt_irq.h>
#endif

#include <system.h>

#include "config.h"
#include "mb.h"
#include "gpio.h"

/**
 * Max number of Neo Pixel channels
 */
#define NP_DEV_NUM      NEOPIXEL_0_DATA_CHANNELS

/**
 * NEOPIXEL_0_CSR registers
 */
#define NP_REG_CH_MSK   0
#define NP_CH_MSK             0x007FFFFF
#define NP_REG_PS_CNT   1
#define NP_PS_CNT_STOP        0x00000040
#define NP_PS_CNT_START       0x00000020
#define NP_PS_CNT_CNT_MSK     0x0000001F
#define NP_REG_TRESET   2
#define NP_REG_T0H      3
#define NP_REG_T1H      4
#define NP_REG_TTOT     5
#define NP_REG_STATUS   7
#define NP_STATUS_IDLE        0
#define NP_STATUS_SHIFTING    1
#define NP_STATUS_RESETTING   2

#define CLOCK   (((float)ALT_CPU_FREQ)/1000000000.0)           // clock in GHz

/**
 */
typedef union {
  struct {
    alt_u8  r;
    alt_u8  g;
    alt_u8  b;
    alt_u8  w;
  };
  alt_u32   c;
}sNpPix, *psNpPix;

/**
 * NeoPixel definition
 * all time is in ns
 */
typedef struct {
  alt_u16   numLEDs;              // number of RGB LEDs in strips
  alt_u8    type;                 // pixel type
  alt_u32   chMsk;                // channel mask bitfield
  alt_u32   cnt;                  // number of bits
  alt_u32   treset;               // reset time
  alt_u32   t0h;                  // high time for 0
  alt_u32   t1h;                  // high time for 1
  alt_u32   ttot;                 // total time for 1/0
  psNpPix   pixels[NP_DEV_NUM];   // holds LED color values
  alt_u16   wtt;                  // word to transfer
}sNpDef, *psNpDef;

/**
 */
alt_u32 npSet(alt_u32 mask, alt_u16 numLEDs, alt_u32 type);
alt_u32 npLedSet(alt_u32 idx, alt_u16 LED, alt_u8 r, alt_u8 g, alt_u8 b, alt_u8 w);
alt_u32 npBrgSet(alt_u32 idx, alt_u8 brg);

alt_u32 npLedShow(void);

#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
void npIsr(void* context);
#else
void npIsr(void* context, alt_u32 id);
#endif

/**
 */
static sNpDef   npDef;

alt_u8  brightness;
alt_u8  rOffset;       // Index of red byte within each 3- or 4-byte pixel
alt_u8  gOffset;       // Index of green byte
alt_u8  bOffset;       // Index of blue byte
alt_u8  wOffset;       // Index of white byte (same as rOffset if no white)

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
  }
  rpc[1] = ret;
}

//#define DBG_PIN (32+13)

/**
 * Initial setup
 */
alt_u32 npSet(alt_u32 mask, alt_u16 numLEDs, alt_u32 type)
{
  int i;

  for (i=0; i<NP_DEV_NUM; i++) {
    if (npDef.pixels[i]) {
      free(npDef.pixels[i]);
    }
    gpioPinMode(32 + i, 0);
    // TODO disable interrupt
  }
  memset(&npDef, 0, sizeof(sNpDef));

  wOffset = ((type >> 6) & 0b11) << 3; // See notes in header file
  rOffset = ((type >> 4) & 0b11) << 3; // regarding R/G/B/W offsets
  gOffset = ((type >> 2) & 0b11) << 3;
  bOffset = ( type       & 0b11) << 3;

  if (wOffset == rOffset) {
    npDef.cnt = 24-1;
  }else{
    npDef.cnt = 32-1;
  }

  if (type & 0x0100) {
    /* SPEED_400K */
    npDef.treset = (alt_u32)((float)300000.0*CLOCK);
    npDef.t0h    = (alt_u32)((float)   480.0*CLOCK);
    npDef.t1h    = (alt_u32)((float)  2000.0*CLOCK);
    npDef.ttot   = (alt_u32)((float)  2480.0*CLOCK);
  }else {
    /* SPEED_800K */
    npDef.treset = (alt_u32)((float)300000.0*CLOCK);
    npDef.t0h    = (alt_u32)((float)   220.0*CLOCK);
    npDef.t1h    = (alt_u32)((float)   750.0*CLOCK);
    npDef.ttot   = (alt_u32)((float)   970.0*CLOCK);
  }

  if (!numLEDs) {
    return 0;
  }
  if (numLEDs < 3) {
    numLEDs = 3;
  }

  npDef.chMsk   = mask & NP_CH_MSK;
  npDef.numLEDs = numLEDs;
  npDef.type    = type;

  for (i=0; i<NP_DEV_NUM; i++) {
    if (npDef.chMsk & (1<<i)) {
      npDef.pixels[i] = (psNpPix)malloc(npDef.numLEDs * sizeof(sNpPix));
      if (!npDef.pixels[i]) {
        return -1;
      }
      gpioPinMode(32 + i, 4);
    }
  }

  while (IORD(NEOPIXEL_0_CSR_BASE, NP_REG_STATUS) != NP_STATUS_IDLE);
  IOWR(NEOPIXEL_0_CSR_BASE, NP_REG_CH_MSK, npDef.chMsk );
  IOWR(NEOPIXEL_0_CSR_BASE, NP_REG_PS_CNT, npDef.cnt   );
  IOWR(NEOPIXEL_0_CSR_BASE, NP_REG_TRESET, npDef.treset);
  IOWR(NEOPIXEL_0_CSR_BASE, NP_REG_T0H   , npDef.t0h   );
  IOWR(NEOPIXEL_0_CSR_BASE, NP_REG_T1H   , npDef.t1h   );
  IOWR(NEOPIXEL_0_CSR_BASE, NP_REG_TTOT  , npDef.ttot  );
#if 1
  memcpy(MB_BASE+0x20, &npDef, sizeof(npDef));
#endif
#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
  alt_ic_isr_register(NEOPIXEL_0_CSR_IRQ_INTERRUPT_CONTROLLER_ID,
                      NEOPIXEL_0_CSR_IRQ,
                      npIsr, NULL, 0);
#else
  alt_irq_register(NEOPIXEL_0_CSR_IRQ, NULL, npIsr);
#endif

#if DBG_PIN
  gpioPinMode(DBG_PIN, 2);
#endif

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
  if ((npDef.chMsk && (1<<idx))==0) {
    return -1;
  }
  if (LED >= npDef.numLEDs) {
    return -1;
  }

  if (brightness) { // See notes in setBrightness()
    r = (r * brightness) >> 8;
    g = (g * brightness) >> 8;
    b = (b * brightness) >> 8;
    w = (w * brightness) >> 8;
  }

  npDef.pixels[idx][LED].c = ((alt_u32)r << rOffset) |
                             ((alt_u32)g << gOffset) |
                             ((alt_u32)b << bOffset) |
                             ((alt_u32)w << wOffset) ;

  return 0;
}

/**
 */
alt_u32 npBrgSet(alt_u32 idx, alt_u8 brg)
{
  brightness = brg;
  return 0;
}

/**
 * Show
 */
alt_u32 npLedShow(void)
{
  int i;

  if (IORD(NEOPIXEL_0_CSR_BASE, NP_REG_STATUS) != NP_STATUS_IDLE) {
    return -1;
  }

  /* write first pixel */
  for (i=0; i<NP_DEV_NUM; i++) {
    if (npDef.chMsk & (1<<i)) {
      IOWR(NEOPIXEL_0_DATA_BASE, i, npDef.pixels[i][0].c);
    }
  }

  /* start transfer */
  IOWR(NEOPIXEL_0_CSR_BASE, NP_REG_PS_CNT, NP_PS_CNT_START | npDef.cnt);

  /* write second pixel */
  for (i=0; i<NP_DEV_NUM; i++) {
    if (npDef.chMsk & (1<<i)) {
      IOWR(NEOPIXEL_0_DATA_BASE, i, npDef.pixels[i][1].c);
    }
  }
  npDef.wtt = npDef.numLEDs - 2;

  return 0;
}

#define SEC_ISR  __attribute__((__section__(".rwdata")))

/**
 */
#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
void SEC_ISR npIsr(void* context)
#else
void SEC_ISR npIsr(void* context, alt_u32 id)
#endif
{
  int i;
  int ofs;
  alt_u32 mask = npDef.chMsk;

#if DBG_PIN
gpioWrite(DBG_PIN, 1);
#endif

  if (npDef.wtt == 0) {
    IOWR(NEOPIXEL_0_CSR_BASE, NP_REG_PS_CNT, NP_PS_CNT_STOP | npDef.cnt);
  }
  else {
    ofs = npDef.numLEDs - npDef.wtt;
    for (i=0; i<NP_DEV_NUM; i++) {
      if (mask & 1) {
        IOWR(NEOPIXEL_0_DATA_BASE, i, npDef.pixels[i][ofs].c);
      }
      mask >>= 1;
    }
    npDef.wtt --;
    IOWR(NEOPIXEL_0_CSR_BASE, NP_REG_PS_CNT, npDef.cnt);
  }
#if DBG_PIN
gpioWrite(DBG_PIN, 0);
#endif
}
