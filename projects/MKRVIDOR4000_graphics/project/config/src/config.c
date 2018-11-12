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
 * @file config.c
 * @brief FPGA configuration
 */

#include "config.h"
#include "fpga.h"

#include "pio.h"
#include "pwm.h"
#include "i2c.h"
#include "np.h"
#include "enc.h"
#ifndef FREE_VERSION
#include "uart.h"
#endif
#include "tspi.h"
#include "gfx.h"
#include "qr.h"

/**
 * FPGA pins assignment
 */
sFpgaPin fpgaPin[] = {
  //port pin giid chn lock
  { 0,  0, 0, 0, 0},
  { 0,  1, 0, 0, 0},
  { 0,  2, 0, 0, 0},
  { 0,  3, 0, 0, 0},
  { 0,  4, 0, 0, 0},
  { 0,  5, 0, 0, 0},
  { 0,  6, 0, 0, 0},
  { 0,  7, 0, 0, 0},
  { 0,  8, 0, 0, 0},
  { 0,  9, 0, 0, 0},
  { 0, 10, 0, 0, 0},
  { 0, 11, 0, 0, 0},
  { 0, 12, 0, 0, 0},
  { 0, 13, 0, 0, 0},
  { 0, 14, 0, 0, 0},
  { 0, 15, 0, 0, 0},
  { 0, 16, 0, 0, 0},
  { 0, 17, 0, 0, 0},
  { 0, 18, 0, 0, 0},
  { 0, 19, 0, 0, 0},
  { 0, 20, 0, 0, 0},
  { 0, 21, 0, 0, 0},
  { 0, 22, 0, 0, 0},

  { 1,  0, 0, 0, 0},
  { 1,  1, 0, 0, 0},
  { 1,  2, 0, 0, 0},
  { 1,  3, 0, 0, 0},
  { 1,  4, 0, 0, 0},
  { 1,  5, 0, 0, 0},
  { 1,  6, 0, 0, 0},
  { 1,  7, 0, 0, 0},
  { 1,  8, 0, 0, 0},
  { 1,  9, 0, 0, 0},
  { 1, 10, 0, 0, 0},
  { 1, 11, 0, 0, 0},
  { 1, 12, 0, 0, 0},
  { 1, 13, 0, 0, 0},
  { 1, 14, 0, 0, 0},
  { 1, 15, 0, 0, 0},
  { 1, 16, 0, 0, 0},
  { 1, 17, 0, 0, 0},
  { 1, 18, 0, 0, 0},
  { 1, 19, 0, 0, 0},
  { 1, 20, 0, 0, 0},
  { 1, 21, 0, 0, 0},
  { 1, 22, 0, 0, 0},
  { 1, 23, 0, 0, 0},
  { 1, 24, 0, 0, 0},

  { 2,  0, 0, 0, 0},  // WM_RESET  SPIWIFI_RESET    NiNa RESETN -> FPGA R1
  { 2,  1, 0, 0, 0},  // WM_PIO2
  { 2,  2, 0, 0, 0},  // WM_PIO3
  { 2,  3, 0, 0, 0},  // WM_PIO4
  { 2,  4, 0, 0, 0},  // WM_PIO5
  { 2,  5, 0, 0, 0},  // WM_PIO7   SPIWIFI_ACK      NiNa GPIO33 -> FPGA P6
  { 2,  6, 0, 0, 0},  // WM_PIO8
  { 2,  7, 0, 0, 0},  // WM_PIO18
  { 2,  8, 0, 0, 0},  // WM_PIO20  NINA UART RTS    NiNa GPIO20 -> FPGA R5
  { 2,  9, 0, 0, 0},  // WM_PIO21  NINA UART CTS    NiNa GPIO21 -> FPGA R6
  { 2, 10, 0, 0, 0},  // WM_PIO27  NINA_GPIO0       NiNa GPIO0  -> FPGA N9
  { 2, 11, 0, 0, 0},  // WM_PIO28  NINA SPI SS      NiNa GPIO5  -> FPGA N11
  { 2, 12, 0, 0, 0},  //
  { 2, 13, 0, 0, 0},  //
  { 2, 14, 0, 0, 0},  //
  { 2, 15, 0, 0, 0},  // NINA UART RX         (64 + 15)
  { 2, 16, 0, 0, 0},  // NINA UART TX         (64 + 16)
  { 2, 17, 0, 0, 0},  //
  { 2, 18, 0, 0, 0},  // NINA SPI CLOCK       (64 + 18)
  { 2, 19, 0, 0, 0},  // NINA SPI MOSI        (64 + 19)
  { 2, 20, 0, 0, 0},  // NINA SPI MISO        (64 + 20)
  { 2, 21, 0, 0, 0},

  { 3,  0, 0, 0, 0},  // CSI I2C SDA
  { 3,  1, 0, 0, 0},  // CSI I2C SCL
  { 3,  2, 0, 0, 0},  // HDMI I2C SDA
  { 3,  3, 0, 0, 0},  // HDMI I2C SCL
  { 3,  4, 0, 0, 0},
  { 3,  5, 0, 0, 0},
  { 3,  6, 0, 0, 0},
  { 3,  7, 0, 0, 0},
  { 3,  8, 0, 0, 0},
  { 3,  9, 0, 0, 0},
  { 3, 10, 0, 0, 0},
  { 3, 11, 0, 0, 0},
  { 3, 12, 0, 0, 0},
  { 3, 13, 0, 0, 0},
  { 3, 14, 0, 0, 0},
  { 3, 15, 0, 0, 0},
  { 3, 16, 0, 0, 0},
  { 3, 17, 0, 0, 0},
  { 3, 18, 0, 0, 0},
  { 3, 19, 0, 0, 0},
  { 3, 20, 0, 0, 0},
};

int fpgaPinsNum = sizeof(fpgaPin)/sizeof(sFpgaPin);

/**
 * PWM pins definition
 */
sFpgaIpPin PWM_0_0_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 0,  0)}};
sFpgaIpPin PWM_0_1_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 0,  1)}};
sFpgaIpPin PWM_0_2_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 0,  2)}};
sFpgaIpPin PWM_0_3_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 0,  3)}};
sFpgaIpPin PWM_0_4_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 0,  4)}};
sFpgaIpPin PWM_0_5_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 0,  5)}};
sFpgaIpPin PWM_0_6_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 0,  6)}};
sFpgaIpPin PWM_0_7_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 0,  7)}};
sFpgaIpPin PWM_0_8_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 0,  8)}};
sFpgaIpPin PWM_0_9_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 0,  9)}};
sFpgaIpPin PWM_0_10_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 0, 10)}};
sFpgaIpPin PWM_0_11_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 0, 11)}};
sFpgaIpPin PWM_0_12_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 0, 12)}};
sFpgaIpPin PWM_0_13_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 0, 13)}};
sFpgaIpPin PWM_0_14_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 0, 14)}};
sFpgaIpPin PWM_0_15_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 0, 15)}};
sFpgaIpPin PWM_0_16_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 0, 16)}};
sFpgaIpPin PWM_0_17_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 0, 17)}};
sFpgaIpPin PWM_0_18_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 0, 18)}};
sFpgaIpPin PWM_0_19_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 0, 19)}};
sFpgaIpPin PWM_0_20_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 0, 20)}};
sFpgaIpPin PWM_0_21_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 0, 21)}};
sFpgaIpPin PWM_0_22_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 0, 22)}};

sFpgaIpChn PWM_0_CHN[] = {
  {NPIN(PWM_0_0_PIN), PWM_0_0_PIN},
  {NPIN(PWM_0_1_PIN), PWM_0_1_PIN},
  {NPIN(PWM_0_2_PIN), PWM_0_2_PIN},
  {NPIN(PWM_0_3_PIN), PWM_0_3_PIN},
  {NPIN(PWM_0_4_PIN), PWM_0_4_PIN},
  {NPIN(PWM_0_5_PIN), PWM_0_5_PIN},
  {NPIN(PWM_0_6_PIN), PWM_0_6_PIN},
  {NPIN(PWM_0_7_PIN), PWM_0_7_PIN},
  {NPIN(PWM_0_8_PIN), PWM_0_8_PIN},
  {NPIN(PWM_0_9_PIN), PWM_0_9_PIN},
  {NPIN(PWM_0_10_PIN), PWM_0_10_PIN},
  {NPIN(PWM_0_11_PIN), PWM_0_11_PIN},
  {NPIN(PWM_0_12_PIN), PWM_0_12_PIN},
  {NPIN(PWM_0_13_PIN), PWM_0_13_PIN},
  {NPIN(PWM_0_14_PIN), PWM_0_14_PIN},
  {NPIN(PWM_0_15_PIN), PWM_0_15_PIN},
  {NPIN(PWM_0_16_PIN), PWM_0_16_PIN},
  {NPIN(PWM_0_17_PIN), PWM_0_17_PIN},
  {NPIN(PWM_0_18_PIN), PWM_0_18_PIN},
  {NPIN(PWM_0_19_PIN), PWM_0_19_PIN},
  {NPIN(PWM_0_20_PIN), PWM_0_20_PIN},
  {NPIN(PWM_0_21_PIN), PWM_0_21_PIN},
  {NPIN(PWM_0_22_PIN), PWM_0_22_PIN},
};

/**
 * I2C pins definition
 */
sFpgaIpPin CSI_I2C_PIN[] = {
  {0, I2C_PIN_FNC_SCL, PIN(0, 3, 0)},
  {0, I2C_PIN_FNC_SDA, PIN(0, 3, 1)}
};
sFpgaIpChn CSI_I2C_CHN[] = {
  {NPIN(CSI_I2C_PIN), CSI_I2C_PIN},
};

sFpgaIpPin HDMI_I2C_PIN[] = {
  {0, I2C_PIN_FNC_SCL, PIN(0, 3, 2)},
  {0, I2C_PIN_FNC_SDA, PIN(0, 3, 3)}
};
sFpgaIpChn HDMI_I2C_CHN[] = {
  {NPIN(HDMI_I2C_PIN), HDMI_I2C_PIN},
};

/**
 * NeoPixel pins definition
 */
sFpgaIpPin NEOPIXEL_0_PIN[]  = { {0, NEOPIXEL_PIN_FNC_OUT, PIN(2, 0,  1)} };
sFpgaIpPin NEOPIXEL_1_PIN[]  = { {0, NEOPIXEL_PIN_FNC_OUT, PIN(2, 0,  2)} };
sFpgaIpPin NEOPIXEL_2_PIN[]  = { {0, NEOPIXEL_PIN_FNC_OUT, PIN(2, 0,  3)} };
sFpgaIpPin NEOPIXEL_3_PIN[]  = { {0, NEOPIXEL_PIN_FNC_OUT, PIN(2, 0,  4)} };
sFpgaIpPin NEOPIXEL_4_PIN[]  = { {0, NEOPIXEL_PIN_FNC_OUT, PIN(2, 0,  5)} };
sFpgaIpPin NEOPIXEL_5_PIN[]  = { {0, NEOPIXEL_PIN_FNC_OUT, PIN(2, 0,  6)} };
sFpgaIpPin NEOPIXEL_6_PIN[]  = { {0, NEOPIXEL_PIN_FNC_OUT, PIN(2, 0,  7)} };
sFpgaIpPin NEOPIXEL_7_PIN[]  = { {0, NEOPIXEL_PIN_FNC_OUT, PIN(2, 0,  8)} };
sFpgaIpPin NEOPIXEL_8_PIN[]  = { {0, NEOPIXEL_PIN_FNC_OUT, PIN(2, 0,  9)} };
sFpgaIpPin NEOPIXEL_9_PIN[]  = { {0, NEOPIXEL_PIN_FNC_OUT, PIN(2, 0, 10)} };
sFpgaIpPin NEOPIXEL_10_PIN[] = { {0, NEOPIXEL_PIN_FNC_OUT, PIN(2, 0, 11)} };
sFpgaIpChn NEOPIXEL_CHN[] = {
  {NPIN(NEOPIXEL_0_PIN), NEOPIXEL_0_PIN},
  {NPIN(NEOPIXEL_1_PIN), NEOPIXEL_1_PIN},
  {NPIN(NEOPIXEL_2_PIN), NEOPIXEL_2_PIN},
  {NPIN(NEOPIXEL_3_PIN), NEOPIXEL_3_PIN},
  {NPIN(NEOPIXEL_4_PIN), NEOPIXEL_4_PIN},
  {NPIN(NEOPIXEL_5_PIN), NEOPIXEL_5_PIN},
  {NPIN(NEOPIXEL_6_PIN), NEOPIXEL_6_PIN},
  {NPIN(NEOPIXEL_7_PIN), NEOPIXEL_7_PIN},
  {NPIN(NEOPIXEL_8_PIN), NEOPIXEL_8_PIN},
  {NPIN(NEOPIXEL_9_PIN), NEOPIXEL_9_PIN},
  {NPIN(NEOPIXEL_10_PIN), NEOPIXEL_10_PIN},
};
sNpPriv np_priv = {
  ((SDRAM_ARBITER_BASE + 0x1C2000)| 0x80000000),
  (SDRAM_ARBITER_SPAN - 0x1C2000)
};

/**
 * Quad Encoder pins definition
 */
sFpgaIpPin ENC_0_PIN[] = {
  {0, ENC_PIN_FNC_A, PIN(0, 0, 1)}, {0, ENC_PIN_FNC_B, PIN(0, 0, 2)},
};
sFpgaIpPin ENC_1_PIN[] = {
  {0, ENC_PIN_FNC_A, PIN(0, 0, 3)}, {0, ENC_PIN_FNC_B, PIN(0, 0, 4)},
};
sFpgaIpPin ENC_2_PIN[] = {
  {0, ENC_PIN_FNC_A, PIN(0, 0, 5)}, {0, ENC_PIN_FNC_B, PIN(0, 0, 6)},
};
sFpgaIpPin ENC_3_PIN[] = {
  {0, ENC_PIN_FNC_A, PIN(0, 0, 7)}, {0, ENC_PIN_FNC_B, PIN(0, 0, 8)},
};
sFpgaIpPin ENC_4_PIN[] = {
  {0, ENC_PIN_FNC_A, PIN(0, 0, 9)}, {0, ENC_PIN_FNC_B, PIN(0, 0, 10)},
};
sFpgaIpPin ENC_5_PIN[] = {
  {0, ENC_PIN_FNC_A, PIN(0, 0, 11)}, {0, ENC_PIN_FNC_B, PIN(0, 0, 12)},
};
sFpgaIpPin ENC_6_PIN[] = {
  {0, ENC_PIN_FNC_A, PIN(0, 0, 13)}, {0, ENC_PIN_FNC_B, PIN(0, 0, 14)},
};
sFpgaIpPin ENC_7_PIN[] = {
  {0, ENC_PIN_FNC_A, PIN(0, 0, 15)}, {0, ENC_PIN_FNC_B, PIN(0, 0, 16)},
};
sFpgaIpPin ENC_8_PIN[] = {
  {0, ENC_PIN_FNC_A, PIN(0, 0, 17)}, {0, ENC_PIN_FNC_B, PIN(0, 0, 18)},
};
sFpgaIpPin ENC_9_PIN[] = {
  {0, ENC_PIN_FNC_A, PIN(0, 0, 19)}, {0, ENC_PIN_FNC_B, PIN(0, 0, 20)},
};

sFpgaIpChn ENC_CHN[] = {
  {NPIN(ENC_0_PIN), ENC_0_PIN},
  {NPIN(ENC_1_PIN), ENC_1_PIN},
  {NPIN(ENC_2_PIN), ENC_2_PIN},
  {NPIN(ENC_3_PIN), ENC_3_PIN},
  {NPIN(ENC_4_PIN), ENC_4_PIN},
  {NPIN(ENC_5_PIN), ENC_5_PIN},
  {NPIN(ENC_6_PIN), ENC_6_PIN},
  {NPIN(ENC_7_PIN), ENC_7_PIN},
  {NPIN(ENC_8_PIN), ENC_8_PIN},
  {NPIN(ENC_9_PIN), ENC_9_PIN},
};

#ifndef FREE_VERSION
/**
 * Uart pins definition
 */
/*
sFpgaIpPin UART_0_PIN[] = {
  {0, UART_PIN_FNC_TX , PIN(2, 0, 1)}, {0, UART_PIN_FNC_RX , PIN(0, 0, 2)},
};
sFpgaIpChn UART_0_CHN[] = {
  {NPIN(UART_0_PIN), UART_0_PIN},
};
UART_DEV(UART_0);
*/
#define NINA_UART_CHNS 1
sFpgaIpPin NINA_UART_PIN[] = {
  {0, UART_PIN_FNC_TX , PIN(2, 2, 15)},
  {0, UART_PIN_FNC_RX , PIN(0, 2, 16)},
  {1, UART_PIN_FNC_TX , PIN(2, 2, 15)},
  {1, UART_PIN_FNC_RX , PIN(0, 2, 16)},
  {1, UART_PIN_FNC_RTS, PIN(2, 2,  9)},
  {1, UART_PIN_FNC_CTS, PIN(0, 2,  8)},
};
sFpgaIpChn NINA_UART_CHN[] = {
  {NPIN(NINA_UART_PIN), NINA_UART_PIN},
};
UART_DEV(NINA_UART);
#endif

/**
 * TSPI pins definition
 */
/*
sFpgaIpPin TSPI_0_PIN[] = {
  {0, TSPI_PIN_FNC_MOSI, PIN(3, 0,  3)},
  {0, TSPI_PIN_FNC_MISO, PIN(0, 0,  7)},
  {0, TSPI_PIN_FNC_CLK , PIN(3, 0,  4)},
  {1, TSPI_PIN_FNC_MOSI, PIN(3, 0,  3)},
  {1, TSPI_PIN_FNC_MISO, PIN(0, 0,  7)},
  {1, TSPI_PIN_FNC_CLK , PIN(3, 0,  4)},
  {1, TSPI_PIN_FNC_SS  , PIN(3, 0,  5)},
};
sFpgaIpChn TSPI_0_CHN[] = {
  {NPIN(TSPI_0_PIN), TSPI_0_PIN},
};
TSPI_DEV(TSPI_0);
*/
#define NINA_TSPI_CHNS 1
sFpgaIpPin NINA_TSPI_PIN[] = {
  {0, TSPI_PIN_FNC_MOSI, PIN(2, 2, 19)},
  {0, TSPI_PIN_FNC_MISO, PIN(0, 2, 20)},
  {0, TSPI_PIN_FNC_CLK , PIN(2, 2, 18)},
};
sFpgaIpChn NINA_TSPI_CHN[] = {
  {NPIN(NINA_TSPI_PIN), NINA_TSPI_PIN},
};
TSPI_DEV(NINA_TSPI);


/**
 */
sFpgaIpRpc fpgaIpRpc[] = {
#if defined(NO_RPC) && (NO_RPC == 1)
#else
  {fpgaRpc, FPGA_VER    },
  {pioRpc , PIO_VER     },
  {pwmRpc , PWM_VER     },
  {i2cRpc , I2C_VER     },
  {npRpc  , NEOPIXEL_VER},
  {encRpc , ENC_VER     },
#ifndef FREE_VERSION
  {uartRpc, UART_VER    },
#else
  {NULL, 0},
#endif
  {tspiRpc, TSPI_VER    },
  {gfxRpc , GFX_VER     },
  {qrRpc  , QR_VER      },
#endif
};

sFpgaIpLoop fpgaIpLoop[] = {
#if defined(NO_RPC) && (NO_RPC == 1)
#else
  {qrLoop},
#endif
};
int fpgaIpLoopNum = (sizeof(fpgaIpLoop)/sizeof(sFpgaIpLoop));

#define FIX_IO_CHNS    21 // TODO

/**
 */
sFpgaIp fpgaIp[] = {

  /**
   * FPGA subsystem MUST BE first item of array
   */
  {0, IP_DISC(0, FPGA_UID), 0, 0},

  /**
   * PIO
   */
  {1, IP_DISC(SAM_PIO_CHANNELS, PIO_UID), SAM_PIO_BASE, NULL},
  {1, IP_DISC(PEX_PIO_CHANNELS, PIO_UID), PEX_PIO_BASE, NULL},
  {1, IP_DISC(WM_PIO_CHANNELS , PIO_UID), WM_PIO_BASE, NULL},
  {1, IP_DISC(FIX_IO_CHNS , FIO_UID), 0, NULL},

  /**
   * PWM
   */
  {2, IP_DISC(SAM_PWM_CHANNELS, PWM_UID), SAM_PWM_BASE, PWM_0_CHN},

  /**
   * I2C
   */
  {3, IP_DISC(1, I2C_UID), CSI_I2C_BASE, CSI_I2C_CHN},
  {3, IP_DISC(1, I2C_UID), HDMI_I2C_BASE, HDMI_I2C_CHN},

  /**
   * NEOPIXEL
   */
  {4, IP_DISC(NEOPIXEL_0_CHANNELS, NEOPIXEL_UID), NEOPIXEL_0_BASE, NEOPIXEL_CHN, &np_priv},

  /**
   * Quad Encoder
   */
  {5, IP_DISC(ENC_CHNS, ENC_UID), QUAD_ENCODER_0_BASE, ENC_CHN},
#ifndef FREE_VERSION
  /**
   * UART
   */
    {6, IP_DISC(NINA_UART_CHNS, UART_UID), NINA_UART_BASE, NINA_UART_CHN, &NINA_UART_DEV},
//  {6, IP_DISC(UART_0_CHNS, UART_UID), UART_0_BASE, UART_0_CHN, &UART_0_DEV},
#endif
  /**
   * TSPI
   */
//  {7, IP_DISC(TSPI_0_CHNS, TSPI_UID), NINA_SPI_BASE, TSPI_0_CHN, &TSPI_0_DEV},
  {7, IP_DISC(NINA_TSPI_CHNS, TSPI_UID), NINA_SPI_BASE, NINA_TSPI_CHN, &NINA_TSPI_DEV},
//  {7, IP_DISC(TSPI_0_CHNS, TSPI_UID), FLASH_SPI_BASE, 0, NULL},
  {7, IP_DISC(0, 0), FLASH_SPI_BASE, 0, 0},

  /**
   * GFX
   */
  {8, IP_DISC(0, GFX_UID), 0, 0, NULL},

  /**
   * QR
   */
  {9, IP_DISC(0, QR_UID), QRCODE_FINDER_0_BASE, 0, NULL},
};
int fpgaIpGiidNum = (sizeof(fpgaIp)/sizeof(sFpgaIp));
