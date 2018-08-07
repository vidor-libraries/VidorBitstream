/*
 * config.h
 *
 *  Created on: Jun 20, 2018
 *      Author: max
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <system.h>

/**
 *
 */
#define FPGA_VERSION 0xC1010102

/**
 * GPIO module
 */
#define GPIO_MODULE       1
#if defined(GPIO_MODULE) && (GPIO_MODULE == 1)
  #define GPIO_GPIO       1
#endif /* defined(GPIO_MODULE) && (GPIO_MODULE == 1) */

/**
 * graphic module
 */
#define GFX_MODULE      0

#if defined(GFX_MODULE) && (GFX_MODULE == 1)
  #define GFX_FB_BASE (alt_u16*)((SDRAM_ARBITER_BASE + \
                              SDRAM_ARBITER_FB_OFFSET*sizeof(short)) | \
                             0x80000000)
  #define GFX_CAM_BASE (alt_u16*)(SDRAM_ARBITER_BASE)
  #define GFX_FB_BASE (alt_u16*)0

  #define GFX_FB_WIDTH  640
  #define GFX_FB_HEIGHT 480

  #define GFX_FONTS       1
  #define GFX_FONT_FILE   "Fonts/FreeMono12pt7b.h"
  #define GFX_FONT_NAME   FreeMono12pt7b
  #define GFX_CMDS        1
#endif /* defined(GFX_MODULE) && (GFX_MODULE == 1) */


/**
 * serial flash module
 */
#define SF_MODULE         1

#if defined(SF_MODULE) && (SF_MODULE == 1)
  #define SF_USE_QSPI       0
  #define SF_SPI_IDX        0
  #define SF_INVERT_BIT     0
  #define SF_SECURITY_CMDS  1
#endif /* defined(SF_MODULE) && (SF_MODULE == 1) */

/**
 * spi module
 */
#define SPI_MODULE  1
#if defined(SPI_MODULE) && (SPI_MODULE == 1)
  #define SPI_DEV_NUM 0
  #define SPI_DEV_CFG \
    /*         BASE   MOSI   MISO    CLK     SS  A*/ \
    {FLASH_SPI_BASE,    -1,    -1,    -1,    -1, 1},
#endif /* defined(SPI_MODULE) && (SPI_MODULE == 1) */

#endif /* CONFIG_H_ */

