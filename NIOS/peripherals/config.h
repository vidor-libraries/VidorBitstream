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
#define FPGA_VERSION 0x01020109

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
#define GFX_MODULE      1
#if defined(GFX_MODULE) && (GFX_MODULE == 1)
  #define GFX_FB_BASE   (alt_u16*)SDRAM_BASE
  #define GFX_CAM_BASE  (alt_u16*)SDRAM_BASE

  #define GFX_FB_WIDTH  640
  #define GFX_FB_HEIGHT 480

  #define GFX_CLEAR       0
  #define GFX_LOGO        0

  #define GFX_FONTS       1
  #define GFX_FONT_FILE   "fonts.h"
  #define GFX_FONT_NAME   Org_01
  //#define FONT_MONO
  //#define FONT_SANS
  //#define FONT_SERIF
  //#define FONT_9PT
  //#define FONT_12PT
  //#define FONT_18PT
  //#define FONT_24PT
  //#define FONT_BOLD
  //#define FONT_ITALIC

  #define GFX_CMDS        1

  /* define graphic context */
  #include "gfx.h"
  #define GFX_GC_NUM    (1 + NEOPIXEL_CHANNELS)
  extern GFXgc *gfxGc[];
/* TODO define graphic context */

#endif /* defined(GFX_MODULE) && (GFX_MODULE == 1) */

/**
 * serial flash module
 */
#define SF_MODULE         0
#if defined(SF_MODULE) && (SF_MODULE == 1)
  #define SF_USE_QSPI       0
  #define SF_SPI_IDX        6
  #define SF_INVERT_BIT     0
  #define SF_SECURITY_CMDS  1
#endif /* defined(SF_MODULE) && (SF_MODULE == 1) */

/**
 */
#define I2C_MODULE    1
#if defined(I2C_MODULE) && (I2C_MODULE == 1)
  #define I2C_DEV_NUM   4
  #define I2C_DEV_BASE  I2C_0_BASE, I2C_1_BASE, I2C_2_BASE, I2C_3_BASE,
#endif /* defined(I2C_MODULE) && (I2C_MODULE == 1) */

/**
 * spi module
 */
#define SPI_MODULE  1
#if defined(SPI_MODULE) && (SPI_MODULE == 1)
  #define SPI_DEV_NUM 6
  #define SPI_DEV_CFG \
    /*         BASE   MOSI   MISO    CLK     SS  A */ \
    {NINA_SPI_BASE ,    -1,    -1,    -1,    -1, 0}, \
    {SPI_0_BASE    , 32+ 3, 32+ 7, 32+ 4, 32+ 5, 1}, \
    {SPI_1_BASE    , 32+ 6, 32+ 8, 32+ 9, 32+10, 1}, \
    {SPI_2_BASE    , 32+11, 32+15, 32+12, 32+13, 1}, \
    {SPI_3_BASE    , 32+14, 32+16, 32+17, 32+18, 1}, \
    {SPI_4_BASE    , 32+19, 32+20, 32+21, 32+22, 1}, \
    {FLASH_SPI_BASE,    -1,    -1,    -1,    -1, 1},
#endif /* defined(SPI_MODULE) && (SPI_MODULE == 1) */

/**
 * uart module
 *  0      2w NINA
 *  1      2w
 *  2      2w rts-cts
 *  3      2w
 *  4      2w rts-cts dtr-dsr
 *  5      2w
 *  6      2w rts-cts
 *  7      2w
 *  8      2w rts-cts dtr-dsr
 */
#define UART_MODULE  1
#if defined(UART_MODULE) && (UART_MODULE == 1)
  #define UART_DEV_NUM 9
  #define UART_PIN_MUX 4
  #define UART_DEV_CFG \
    /*      name  cap     tx     rx    rts    cts    dtr    dsr*/\
    {NINA_UART_NAME, 0x0, 64+15, 64+16,    -1,    -1,    -1,    -1},\
    {UART_0_NAME   , 0x0, 32+ 1, 32+ 2,    -1,    -1,    -1,    -1},\
    {UART_1_NAME   , 0x1, 32+ 3, 32+ 4, 32+ 2, 32+ 1,    -1,    -1},\
    {UART_2_NAME   , 0x0, 32+ 5, 32+ 6,    -1,    -1,    -1,    -1},\
    {UART_3_NAME   , 0x3, 32+ 7, 32+ 8, 32+ 6, 32+ 5, 32+ 4, 32+ 3},\
    {UART_4_NAME   , 0x0, 32+ 9, 32+10,    -1,    -1,    -1,    -1},\
    {UART_5_NAME   , 0x1, 32+11, 32+12, 32+10, 32+ 9,    -1,    -1},\
    {UART_6_NAME   , 0x0, 32+13, 32+14,    -1,    -1,    -1,    -1},\
    {UART_7_NAME   , 0x3, 32+15, 32+16, 32+14, 32+13, 32+12, 32+11},

#endif /* defined(UART_MODULE) && (UART_MODULE == 1) */

/**
 * qrcode module
 */
#define QR_MODULE 0
#if defined(QR_MODULE) && (QR_MODULE == 1)
#endif /* defined(QR_MODULE) && (QR_MODULE == 1) */

/**
 * SDRAM module
 */
#define SDRAM_MODULE         1
#if defined(SDRAM_MODULE) && (SDRAM_MODULE == 1)
//  #define SDRAM_BASE  (SDRAM_ARBITER_BASE | 0x80000000)
//  #define SDRAM_SIZE  SDRAM_ARBITER_SPAN
  //#define SDRAM_BASE  (0 | 0x80000000)
  //#define SDRAM_SIZE  0x00800000
  #define SDRAM_SIZE  SDRAM_SPAN
#endif /* defined(SDRAM_MODULE) && (SDRAM_MODULE == 1) */

/**
 * Neo Pixels module
 */
#define NP_MODULE         1
#if defined(NP_MODULE) && (NP_MODULE == 1)
  #define NP_DEV_NUM      NEOPIXEL_CHANNELS
  #define NP_CSR_BASE     NEOPIXEL_BASE
  #define NP_MEM_BASE     (SDRAM_BASE + 0x00000000)
  #define NP_MEM_SIZE     (SDRAM_SPAN - 0x00000000)
  #define NP_PIN_MUX      5
  #define NP_PIN          32+7, 32+8, 32+15, 32+16, 32+20
#endif /* defined(NP_MODULE) && (NP_MODULE == 1) */

/**
 * Qudrature encoder module
 */
#define ENC_MODULE  1
#if defined(ENC_MODULE) && (ENC_MODULE == 1)
  #define ENC_DEV_NUM     11
  #define ENC_BASE        QUAD_ENCODER_0_BASE
#endif /* defined(ENC_MODULE) && (ENC_MODULE == 1) */

/**
 * register module
 */
#define REG_MODULE  1

/**
 * timer module
 */
#define TMR_MODULE  1
#if defined(TMR_MODULE) && (TMR_MODULE == 1)
  #define TMR_BASE        TIMER_0_BASE
  #define TMR_FREQ        TIMER_0_FREQ
  #define TMR_IRQ_CTR_ID  0
  #define TMR_IRQ         TIMER_0_IRQ
#endif /* defined(TMR_MODULE) && (TMR_MODULE == 1) */

void configInit(void);

#endif /* CONFIG_H_ */

