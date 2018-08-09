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
#define FPGA_VERSION 0x01010105


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

  #define GFX_FB_BASE (alt_u16*)((SDRAM_ARBITER_BASE + \
                              SDRAM_ARBITER_FB_OFFSET*sizeof(short)) | \
                             0x80000000)
  #define GFX_CAM_BASE    (alt_u16*)(SDRAM_ARBITER_BASE)

  #define GFX_FB_WIDTH    640
  #define GFX_FB_HEIGHT   480

  #define GFX_LOGO        1

  #define GFX_FONTS       1
  #define GFX_FONT_FILE   "Fonts/FreeMono12pt7b.h"
  #define GFX_FONT_NAME   FreeMono12pt7b

  #define GFX_CMDS        1
#endif /* defined(GFX_MODULE) && (GFX_MODULE == 1) */

/**
 * serial flash module
 */
#define SF_MODULE         0
#if defined(SF_MODULE) && (SF_MODULE == 1)
  #define SF_USE_QSPI       1
  #define SF_SPI_IDX        1
  #define SF_INVERT_BIT     0
  #define SF_SECURITY_CMDS  1
#endif /* defined(SF_MODULE) && (SF_MODULE == 1) */

/**
 */
#define I2C_MODULE    1

#if defined(I2C_MODULE) && (I2C_MODULE == 1)
  #define I2C_DEV_NUM 2
  #define I2C_DEV_BASE   CSI_I2C_BASE, HDMI_I2C_BASE,
#endif /* defined(I2C_MODULE) && (I2C_MODULE == 1) */

/**
 * spi module
 */
#define SPI_MODULE  1
#if defined(SPI_MODULE) && (SPI_MODULE == 1)
  #define SPI_DEV_NUM 1
  #define SPI_INTERRUPT_CONTROLLER_ID NINA_IRQ_INTERRUPT_CONTROLLER_ID
  #define SPI_DEV_CFG \
    /*    BASE        MOSI   MISO    CLK     SS  A */ \
    {NINA_SPI_BASE ,    -1,    -1,    -1,    -1, 0}, \
    {FLASH_SPI_BASE,    -1,    -1,    -1,    -1, 1},
  #define SPI_SS_AUTO  0
#endif /* defined(SPI_MODULE) && (SPI_MODULE == 1) */

/**
 * uart module
 */
#define UART_MODULE  1
#if defined(UART_MODULE) && (UART_MODULE == 1)
  #define UART_DEV_NUM 1
  #define UART_PIN_MUX 4
  #define UART_DEV_CFG \
    /*      name  cap     tx  rx rts cts dtr dsr*/\
    {NINA_UART_NAME, 0x0, -1, -1, -1, -1, -1, -1},

#endif /* defined(UART_MODULE) && (UART_MODULE == 1) */

/**
 * qrcode module
 */
#define QR_MODULE 1
#if defined(QR_MODULE) && (QR_MODULE == 1)
#endif /* defined(QR_MODULE) && (QR_MODULE == 1) */

/**
 * SDRAM module
 */
#define SDRAM_MODULE         1

#if defined(SDRAM_MODULE) && (SDRAM_MODULE == 1)
  #define SDRAM_BASE  (SDRAM_ARBITER_BASE | 0x80000000)
  #define SDRAM_SIZE  SDRAM_ARBITER_SPAN
  //#define SDRAM_BASE  (0 | 0x80000000)
  //#define SDRAM_SIZE  0x00800000
  //#define SDRAM_SIZE  SDRAM_SPAN
#endif /* defined(SDRAM_MODULE) && (SDRAM_MODULE == 1) */

/**
 * Neo Pixels module
 */
#define NP_MODULE         1

#if defined(NP_MODULE) && (NP_MODULE == 1)
  #define NP_DEV_NUM      NEOPIXEL_0_CHANNELS
  #define NP_CSR_BASE     NEOPIXEL_0_BASE
  #define NP_MEM_BASE     ((SDRAM_ARBITER_BASE + 0x00200000)  | 0x80000000)
  #define NP_MEM_SIZE      (SDRAM_ARBITER_SPAN - 0x00200000)
  #define NP_PIN_MUX      4
  #define NP_PIN          32+1, 32+2, 32+3, 32+4, 32+5, 32+6, 32+7, 32+8, \
                          32+9, 32+10, 32+11,
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

#endif /* CONFIG_H_ */

