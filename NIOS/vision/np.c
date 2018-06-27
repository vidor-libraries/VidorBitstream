/**
 *
 * @file np.c
 * @brief NeoPixel driver
 *
 */

#include <alt_types.h>

#include "mb.h"


#define NP_FLG_SHOW 0x00000001



/**
 */
typedef struct {
  union {
    struct {
      alt_u8  r;
      alt_u8  g;
      alt_u8  b;
      alt_u8  w;
    };
    alt_u32   c;
  }
}sNpPix, *psNpPix;

/**
 * NeoPixel definition
 */
typedef struct {
  alt_u16   numLEDs;       // Number of RGB LEDs in strip
  alt_u16   numBytes;      // Size of 'pixels' buffer below (3 or 4 bytes/pixel)

  psNpPix   pixels;        // Holds LED color values (3 or 4 bytes each)

  alt_u8    rOffset;       // Index of red byte within each 3- or 4-byte pixel
  alt_u8    gOffset;       // Index of green byte
  alt_u8    bOffset;       // Index of blue byte
  alt_u8    wOffset;       // Index of white byte (same as rOffset if no white)

  alt_u8    is800KHz;      // ...true if 800 KHz pixels
  alt_u8    brightness;

}sNpDef, *psNpDef;

/**
 */
alt_u32 npSet(alt_u16 numLEDs, alt_u16 numBpp, alt_u8 type, alt_u8 speed);
alt_u32 npLedSet(alt_u16 LED, alt_u32 color, alt_u32 flags);
alt_u32 npLedShow(void);

/**
 */
static sNpDef npDef;

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
    ret = npSet(rpc[1], rpc[2], rpc[3], rpc[4]);
    break;
  case 2:
    /* set single LED */
    ret = npLedSet(rpc[1], rpc[2], rpc[3]);
    break;
  case 3:
    /* show actual configuration */
    ret = npLedShow();
    break;
  }
  rpc[1] = ret;
}

/**
 */
alt_u32 npSet(alt_u16 numLEDs, alt_u16 numBpp, alt_u8 type, alt_u8 speed)
{
  return 0;
}

/**
 */
alt_u32 npLedSet(alt_u16 LED, alt_u32 color, alt_u32 flags)
{
  if (LED >= npDef.numLEDs) {
    return -1;
  }
  npDef.pixels[LED].r = color;
  npDef.pixels[LED].g = color;
  npDef.pixels[LED].b = color;
  npDef.pixels[LED].w = color;
  if (flags & NP_FLG_SHOW) {
    // call IP
    return npDef.pixels[LED].c;
  }
  return 0;
}

/**
 */
alt_u32 npLedShow(void)
{
  return 0;
}

