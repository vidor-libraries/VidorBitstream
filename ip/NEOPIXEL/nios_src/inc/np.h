/**
 *
 * @file np.h
 * @brief NeoPixel driver
 *
 */
#ifndef NP_H_
#define NP_H_

/**
 * configuration options
 */
#define NP_USE_TMR    1
#define NP_GFX        1
#define NP_CLOCK   (((float)ALT_CPU_FREQ)/1000000000.0)       // clock in GHz

/**
 * IP configuration
 */
#define NEOPIXEL_UID  0xa0894
#define NEOPIXEL_IP_VER   0x0302
#define NEOPIXEL_DRV_VER  0x0406
#define NEOPIXEL_VER      (((NEOPIXEL_IP_VER)<<16)|(NEOPIXEL_DRV_VER))

#define NEOPIXEL_PIN_FNC_OUT PIN_FNC(0x01, PIN_DIR_O)

/**
 */
typedef struct {
  alt_u32   mem_base;
  alt_u32   mem_size;
}sNpPriv, *psNpPriv;

void npRpc(void);

#if defined(NP_GFX) && (NP_GFX == 1)
  #include "gfx.h"
  extern GFXgc npGfxGc[NEOPIXEL_0_CHANNELS];
#endif

#endif /* NP_H_ */

