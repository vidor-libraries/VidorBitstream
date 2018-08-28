/*
 * config.c
 *
 *  Created on: Aug 22, 2018
 *      Author: max
 */

#include "config.h"

/**
 * graphic module
 */
#if defined(GFX_MODULE) && (GFX_MODULE == 1)
#include "gfx.h"
GFXgc *gfxGc[GFX_GC_NUM];

extern GFXgc gfxDefaultGc;

#if defined(NP_MODULE) && (NP_MODULE == 1)
extern GFXgc gfxNpGc[NP_DEV_NUM];
#endif /* defined(NP_MODULE) && (NP_MODULE == 1) */

#endif /* defined(GFX_MODULE) && (GFX_MODULE == 1) */


/**
 */
void configInit(void)
{
#if defined(GFX_MODULE) && (GFX_MODULE == 1)
  gfxGc[0] = &gfxDefaultGc;

#if defined(NP_MODULE) && (NP_MODULE == 1)
  int i;

  for (i=0; i<NP_DEV_NUM; i++){
    gfxGc[1+i] = &gfxNpGc[i];
  }
#endif /* defined(NP_MODULE) && (NP_MODULE == 1) */


#endif /* defined(GFX_MODULE) && (GFX_MODULE == 1) */

}