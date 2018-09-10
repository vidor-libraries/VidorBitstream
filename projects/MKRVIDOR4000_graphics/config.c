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
#include "uart.h"
#include "tspi.h"

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
  { 0, 23, 0, 0, 0},
  { 0, 24, 0, 0, 0},

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

  { 2,  0, 0, 0, 0},
  { 2,  1, 0, 0, 0},
  { 2,  2, 0, 0, 0},
  { 2,  3, 0, 0, 0},
  { 2,  4, 0, 0, 0},
  { 2,  5, 0, 0, 0},
  { 2,  6, 0, 0, 0},
  { 2,  7, 0, 0, 0},
  { 2,  8, 0, 0, 0},
  { 2,  9, 0, 0, 0},
  { 2, 10, 0, 0, 0},
  { 2, 11, 0, 0, 0},
  { 2, 12, 0, 0, 0},
  { 2, 13, 0, 0, 0},
  { 2, 14, 0, 0, 0},
  { 2, 15, 0, 0, 0},
  { 2, 16, 0, 0, 0},
  { 2, 17, 0, 0, 0},
  { 2, 18, 0, 0, 0},

};

int fpgaPinsNum = sizeof(fpgaPin)/sizeof(sFpgaPin);

/**
 * PWM pins definition
 */
sFpgaIpPin PWM_0_0_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 1,  0)}};
sFpgaIpPin PWM_0_1_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 1,  1)}};
sFpgaIpPin PWM_0_2_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 1,  2)}};
sFpgaIpPin PWM_0_3_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 1,  3)}};
sFpgaIpPin PWM_0_4_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 1,  4)}};
sFpgaIpPin PWM_0_5_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 1,  5)}};
sFpgaIpPin PWM_0_6_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 1,  6)}};
sFpgaIpPin PWM_0_7_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 1,  7)}};
sFpgaIpPin PWM_0_8_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 1,  8)}};
sFpgaIpPin PWM_0_9_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 1,  9)}};

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
};

sFpgaIpPin PWM_1_0_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 2, 10)}};
sFpgaIpPin PWM_1_1_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 2, 11)}};
sFpgaIpPin PWM_1_2_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 2, 12)}};
sFpgaIpPin PWM_1_3_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 2, 13)}};
sFpgaIpPin PWM_1_4_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 2, 14)}};
sFpgaIpPin PWM_1_5_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 2, 15)}};
sFpgaIpPin PWM_1_6_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 2, 16)}};
sFpgaIpPin PWM_1_7_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 2, 17)}};
sFpgaIpPin PWM_1_8_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 2, 18)}};
sFpgaIpPin PWM_1_9_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 2, 19)}};

sFpgaIpChn PWM_1_CHN[] = {
  {NPIN(PWM_1_0_PIN), PWM_1_0_PIN},
  {NPIN(PWM_1_1_PIN), PWM_1_1_PIN},
  {NPIN(PWM_1_2_PIN), PWM_1_2_PIN},
  {NPIN(PWM_1_3_PIN), PWM_1_3_PIN},
  {NPIN(PWM_1_4_PIN), PWM_1_4_PIN},
  {NPIN(PWM_1_5_PIN), PWM_1_5_PIN},
  {NPIN(PWM_1_6_PIN), PWM_1_6_PIN},
  {NPIN(PWM_1_7_PIN), PWM_1_7_PIN},
  {NPIN(PWM_1_8_PIN), PWM_1_8_PIN},
  {NPIN(PWM_1_9_PIN), PWM_1_9_PIN},
};

sFpgaIpPin PWM_2_0_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 3, 20)}};
sFpgaIpPin PWM_2_1_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 3, 21)}};
sFpgaIpPin PWM_2_2_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 3, 22)}};
sFpgaIpPin PWM_2_3_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 3, 23)}};
sFpgaIpPin PWM_2_4_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 3, 24)}};
sFpgaIpPin PWM_2_5_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 3, 25)}};
sFpgaIpPin PWM_2_6_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 3, 26)}};
sFpgaIpPin PWM_2_7_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 3, 27)}};
sFpgaIpPin PWM_2_8_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 3, 28)}};
sFpgaIpPin PWM_2_9_PIN[] = {{0, PWM_PIN_FNC_OUT, PIN(1, 3, 29)}};

sFpgaIpChn PWM_2_CHN[] = {
  {NPIN(PWM_2_0_PIN), PWM_2_0_PIN},
  {NPIN(PWM_2_1_PIN), PWM_2_1_PIN},
  {NPIN(PWM_2_2_PIN), PWM_2_2_PIN},
  {NPIN(PWM_2_3_PIN), PWM_2_3_PIN},
  {NPIN(PWM_2_4_PIN), PWM_2_4_PIN},
  {NPIN(PWM_2_5_PIN), PWM_2_5_PIN},
  {NPIN(PWM_2_6_PIN), PWM_2_6_PIN},
  {NPIN(PWM_2_7_PIN), PWM_2_7_PIN},
  {NPIN(PWM_2_8_PIN), PWM_2_8_PIN},
  {NPIN(PWM_2_9_PIN), PWM_2_9_PIN},
};

/**
 * I2C pins definition
 */
sFpgaIpPin I2C_0_0_PIN[] = {
  {0, I2C_PIN_FNC_SCL, PIN(0, 4, 1)},
  {0, I2C_PIN_FNC_SDA, PIN(0, 4, 2)}
};

sFpgaIpChn I2C_0_CHN[] = {
  {NPIN(I2C_0_0_PIN), I2C_0_0_PIN},
};

sFpgaIpPin I2C_1_0_PIN[] = {
  {0, I2C_PIN_FNC_SCL, PIN(0, 4, 3)},
  {0, I2C_PIN_FNC_SDA, PIN(0, 4, 4)}
};

sFpgaIpChn I2C_1_CHN[] = {
  {NPIN(I2C_1_0_PIN), I2C_1_0_PIN},
};

/**
 * NeoPixel pins definition
 */
sFpgaIpPin NEOPIXEL_0_PIN[] = {
  {0, NEOPIXEL_PIN_FNC_OUT, PIN(3, 1, 7)},
};
sFpgaIpPin NEOPIXEL_1_PIN[] = {
  {0, NEOPIXEL_PIN_FNC_OUT, PIN(3, 1, 8)},
};
sFpgaIpPin NEOPIXEL_2_PIN[] = {
  {0, NEOPIXEL_PIN_FNC_OUT, PIN(3, 1, 15)},
};
sFpgaIpPin NEOPIXEL_3_PIN[] = {
  {0, NEOPIXEL_PIN_FNC_OUT, PIN(3, 1, 16)},
};
sFpgaIpPin NEOPIXEL_4_PIN[] = {
  {0, NEOPIXEL_PIN_FNC_OUT, PIN(3, 1, 20)},
};
sFpgaIpChn NEOPIXEL_CHN[] = {
  {NPIN(NEOPIXEL_0_PIN), NEOPIXEL_0_PIN},
  {NPIN(NEOPIXEL_1_PIN), NEOPIXEL_1_PIN},
  {NPIN(NEOPIXEL_2_PIN), NEOPIXEL_2_PIN},
  {NPIN(NEOPIXEL_3_PIN), NEOPIXEL_3_PIN},
  {NPIN(NEOPIXEL_4_PIN), NEOPIXEL_4_PIN},
};

/**
 * Quad Encoder pins definition
 */
sFpgaIpPin ENC_0_PIN[] = {
  {0, ENC_PIN_FNC_A, PIN(0, 1, 1)}, {0, ENC_PIN_FNC_B, PIN(0, 1, 2)},
};
sFpgaIpPin ENC_1_PIN[] = {
  {0, ENC_PIN_FNC_A, PIN(0, 1, 3)}, {0, ENC_PIN_FNC_B, PIN(0, 1, 4)},
};
sFpgaIpPin ENC_2_PIN[] = {
  {0, ENC_PIN_FNC_A, PIN(0, 1, 5)}, {0, ENC_PIN_FNC_B, PIN(0, 1, 6)},
};
sFpgaIpPin ENC_3_PIN[] = {
  {0, ENC_PIN_FNC_A, PIN(0, 1, 7)}, {0, ENC_PIN_FNC_B, PIN(0, 1, 8)},
};
sFpgaIpPin ENC_4_PIN[] = {
  {0, ENC_PIN_FNC_A, PIN(0, 1, 9)}, {0, ENC_PIN_FNC_B, PIN(0, 1, 10)},
};
sFpgaIpPin ENC_5_PIN[] = {
  {0, ENC_PIN_FNC_A, PIN(0, 1, 11)}, {0, ENC_PIN_FNC_B, PIN(0, 1, 12)},
};
sFpgaIpPin ENC_6_PIN[] = {
  {0, ENC_PIN_FNC_A, PIN(0, 1, 13)}, {0, ENC_PIN_FNC_B, PIN(0, 1, 14)},
};
sFpgaIpPin ENC_7_PIN[] = {
  {0, ENC_PIN_FNC_A, PIN(0, 1, 15)}, {0, ENC_PIN_FNC_B, PIN(0, 1, 16)},
};
sFpgaIpPin ENC_8_PIN[] = {
  {0, ENC_PIN_FNC_A, PIN(0, 1, 17)}, {0, ENC_PIN_FNC_B, PIN(0, 1, 18)},
};
sFpgaIpPin ENC_9_PIN[] = {
  {0, ENC_PIN_FNC_A, PIN(0, 1, 19)}, {0, ENC_PIN_FNC_B, PIN(0, 1, 20)},
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

/**
 * Uart pins definition
 */
sFpgaIpPin UART_0_PIN[] = {
  {0, UART_PIN_FNC_TX , PIN(2, 1, 1)}, {0, UART_PIN_FNC_RX , PIN(0, 1, 2)},
};
sFpgaIpChn UART_0_CHN[] = {
  {NPIN(UART_0_PIN), UART_0_PIN},
};
UART_DEV(UART_0);
sFpgaIpPin UART_1_PIN[] = {
  {0, UART_PIN_FNC_TX , PIN(2, 1, 3)}, {0, UART_PIN_FNC_RX , PIN(0, 1, 4)},
  {1, UART_PIN_FNC_TX , PIN(2, 1, 3)}, {1, UART_PIN_FNC_RX , PIN(0, 1, 4)},
  {1, UART_PIN_FNC_RTS, PIN(2, 1, 2)}, {1, UART_PIN_FNC_CTS, PIN(0, 1, 1)},
};
sFpgaIpChn UART_1_CHN[] = {
  {NPIN(UART_1_PIN), UART_1_PIN},
};
UART_DEV(UART_1);
sFpgaIpPin UART_2_PIN[] = {
  {0, UART_PIN_FNC_TX , PIN(2, 1, 5)}, {0, UART_PIN_FNC_RX , PIN(0, 1, 6)},
};
sFpgaIpChn UART_2_CHN[] = {
  {NPIN(UART_2_PIN), UART_2_PIN},
};
UART_DEV(UART_2);
sFpgaIpPin UART_3_PIN[] = {
  {0, UART_PIN_FNC_TX , PIN(2, 1, 7)}, {0, UART_PIN_FNC_RX , PIN(0, 1, 8)},
  {1, UART_PIN_FNC_TX , PIN(2, 1, 7)}, {1, UART_PIN_FNC_RX , PIN(0, 1, 8)},
  {1, UART_PIN_FNC_RTS, PIN(2, 1, 6)}, {1, UART_PIN_FNC_CTS, PIN(0, 1, 5)},
  {2, UART_PIN_FNC_TX , PIN(2, 1, 7)}, {2, UART_PIN_FNC_RX , PIN(0, 1, 8)},
  {2, UART_PIN_FNC_RTS, PIN(2, 1, 6)}, {2, UART_PIN_FNC_CTS, PIN(0, 1, 5)},
  {2, UART_PIN_FNC_DTR, PIN(2, 1, 4)}, {2, UART_PIN_FNC_DSR, PIN(0, 1, 3)},
};
sFpgaIpChn UART_3_CHN[] = {
  {NPIN(UART_3_PIN), UART_3_PIN},
};
UART_DEV(UART_3);
sFpgaIpPin UART_4_PIN[] = {
  {0, UART_PIN_FNC_TX , PIN(2, 1, 9)}, {0, UART_PIN_FNC_RX , PIN(0, 1, 10)},
};
sFpgaIpChn UART_4_CHN[] = {
  {NPIN(UART_4_PIN), UART_4_PIN},
};
UART_DEV(UART_4);
sFpgaIpPin UART_5_PIN[] = {
  {0, UART_PIN_FNC_TX , PIN(2, 1, 11)}, {0, UART_PIN_FNC_RX , PIN(0, 1, 12)},
  {1, UART_PIN_FNC_TX , PIN(2, 1, 11)}, {1, UART_PIN_FNC_RX , PIN(0, 1, 12)},
  {1, UART_PIN_FNC_RTS, PIN(2, 1, 10)}, {1, UART_PIN_FNC_CTS, PIN(0, 1,  9)},
};
sFpgaIpChn UART_5_CHN[] = {
  {NPIN(UART_5_PIN), UART_5_PIN},
};
UART_DEV(UART_5);
sFpgaIpPin UART_6_PIN[] = {
  {0, UART_PIN_FNC_TX , PIN(2, 1, 13)}, {0, UART_PIN_FNC_RX , PIN(0, 1, 14)},
};
sFpgaIpChn UART_6_CHN[] = {
  {NPIN(UART_6_PIN), UART_6_PIN},
};
UART_DEV(UART_6);
sFpgaIpPin UART_7_PIN[] = {
  {0, UART_PIN_FNC_TX , PIN(2, 1, 15)}, {0, UART_PIN_FNC_RX , PIN(0, 1, 16)},
  {1, UART_PIN_FNC_TX , PIN(2, 1, 15)}, {1, UART_PIN_FNC_RX , PIN(0, 1, 16)},
  {1, UART_PIN_FNC_RTS, PIN(2, 1, 14)}, {1, UART_PIN_FNC_CTS, PIN(0, 1, 13)},
  {2, UART_PIN_FNC_TX , PIN(2, 1, 15)}, {2, UART_PIN_FNC_RX , PIN(0, 1, 16)},
  {2, UART_PIN_FNC_RTS, PIN(2, 1, 14)}, {2, UART_PIN_FNC_CTS, PIN(0, 1, 13)},
  {2, UART_PIN_FNC_DTR, PIN(2, 1, 12)}, {2, UART_PIN_FNC_DSR, PIN(0, 1, 11)},
};
sFpgaIpChn UART_7_CHN[] = {
  {NPIN(UART_7_PIN), UART_7_PIN},
};
UART_DEV(UART_7);

/**
 * TSPI pins definition
 */
sFpgaIpPin TSPI_0_PIN[] = {
  {0, TSPI_PIN_FNC_MOSI, PIN(3, 1,  3)},
  {0, TSPI_PIN_FNC_MISO, PIN(0, 1,  7)},
  {0, TSPI_PIN_FNC_CLK , PIN(3, 1,  4)},
  {0, TSPI_PIN_FNC_SS  , PIN(3, 1,  5)},
};
sFpgaIpChn TSPI_0_CHN[] = {
  {NPIN(TSPI_0_PIN), TSPI_0_PIN},
};
TSPI_DEV(TSPI_0);
sFpgaIpPin TSPI_1_PIN[] = {
  {0, TSPI_PIN_FNC_MOSI, PIN(3, 1,  6)},
  {0, TSPI_PIN_FNC_MISO, PIN(0, 1,  8)},
  {0, TSPI_PIN_FNC_CLK , PIN(3, 1,  9)},
  {0, TSPI_PIN_FNC_SS  , PIN(3, 1, 10)},
};
sFpgaIpChn TSPI_1_CHN[] = {
  {NPIN(TSPI_1_PIN), TSPI_1_PIN},
};
TSPI_DEV(TSPI_1);
sFpgaIpPin TSPI_2_PIN[] = {
  {0, TSPI_PIN_FNC_MOSI, PIN(3, 1, 11)},
  {0, TSPI_PIN_FNC_MISO, PIN(0, 1, 15)},
  {0, TSPI_PIN_FNC_CLK , PIN(3, 1, 12)},
  {0, TSPI_PIN_FNC_SS  , PIN(3, 1, 13)},
};
sFpgaIpChn TSPI_2_CHN[] = {
  {NPIN(TSPI_2_PIN), TSPI_2_PIN},
};
TSPI_DEV(TSPI_2);
sFpgaIpPin TSPI_3_PIN[] = {
  {0, TSPI_PIN_FNC_MOSI, PIN(3, 1, 14)},
  {0, TSPI_PIN_FNC_MISO, PIN(0, 1, 16)},
  {0, TSPI_PIN_FNC_CLK , PIN(3, 1, 17)},
  {0, TSPI_PIN_FNC_SS  , PIN(3, 1, 18)},
};
sFpgaIpChn TSPI_3_CHN[] = {
  {NPIN(TSPI_3_PIN), TSPI_3_PIN},
};
TSPI_DEV(TSPI_3);
sFpgaIpPin TSPI_4_PIN[] = {
  {0, TSPI_PIN_FNC_MOSI, PIN(3, 1, 19)},
  {0, TSPI_PIN_FNC_MISO, PIN(0, 1, 20)},
  {0, TSPI_PIN_FNC_CLK , PIN(3, 1, 21)},
  {0, TSPI_PIN_FNC_SS  , PIN(3, 1, 22)},
};
sFpgaIpChn TSPI_4_CHN[] = {
  {NPIN(TSPI_4_PIN), TSPI_4_PIN},
};
TSPI_DEV(TSPI_4);

/**
 */
sFpgaIpRpc fpgaIpRpc[] = {
  {fpgaRpc, FPGA_VER    },
  {pioRpc , PIO_VER     },
  {pwmRpc , PWM_VER     },
  {i2cRpc , I2C_VER     },
  {npRpc  , NEOPIXEL_VER},
  {encRpc , ENC_VER     },
  {uartRpc, UART_VER    },
  {tspiRpc, TSPI_VER    },
};

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
  {1, IP_DISC(PIO_0_CHNS, PIO_UID), PIO_0_BASE, 0},
  {1, IP_DISC(PIO_1_CHNS, PIO_UID), PIO_1_BASE, 0},
  {1, IP_DISC(PIO_2_CHNS, PIO_UID), PIO_2_BASE, 0},

  /**
   * PWM
   */
  {2, IP_DISC(PWM_0_CHNS, PWM_UID), PWM_0_BASE, PWM_0_CHN},
  {2, IP_DISC(PWM_1_CHNS, PWM_UID), PWM_1_BASE, PWM_1_CHN},
  {2, IP_DISC(PWM_2_CHNS, PWM_UID), PWM_2_BASE, PWM_2_CHN},

  /**
   * I2C
   */
  {3, IP_DISC(I2C_0_CHNS, I2C_UID), I2C_0_BASE, I2C_0_CHN},
  {3, IP_DISC(I2C_1_CHNS, I2C_UID), I2C_1_BASE, I2C_1_CHN},

  /**
   * NEOPIXEL
   */
  {4, IP_DISC(NEOPIXEL_CHANNELS, NEOPIXEL_UID), NEOPIXEL_BASE, NEOPIXEL_CHN},

  /**
   * Quad Encoder
   */
  {5, IP_DISC(ENC_CHNS, ENC_UID), ENC_BASE, ENC_CHN},

  /**
   * UART
   */
  {6, IP_DISC(UART_0_CHNS, UART_UID), UART_0_BASE, UART_0_CHN, &UART_0_DEV},
  {6, IP_DISC(UART_1_CHNS, UART_UID), UART_1_BASE, UART_1_CHN, &UART_1_DEV},
  {6, IP_DISC(UART_2_CHNS, UART_UID), UART_2_BASE, UART_2_CHN, &UART_2_DEV},
  {6, IP_DISC(UART_3_CHNS, UART_UID), UART_3_BASE, UART_3_CHN, &UART_3_DEV},
  {6, IP_DISC(UART_4_CHNS, UART_UID), UART_4_BASE, UART_4_CHN, &UART_4_DEV},
  {6, IP_DISC(UART_5_CHNS, UART_UID), UART_5_BASE, UART_5_CHN, &UART_5_DEV},
  {6, IP_DISC(UART_6_CHNS, UART_UID), UART_6_BASE, UART_6_CHN, &UART_6_DEV},
  {6, IP_DISC(UART_7_CHNS, UART_UID), UART_7_BASE, UART_7_CHN, &UART_7_DEV},

  /**
   * TSPI
   */
  {7, IP_DISC(TSPI_0_CHNS, TSPI_UID), TSPI_0_BASE, TSPI_0_CHN, &TSPI_0_DEV},
  {7, IP_DISC(TSPI_1_CHNS, TSPI_UID), TSPI_1_BASE, TSPI_1_CHN, &TSPI_1_DEV},
  {7, IP_DISC(TSPI_2_CHNS, TSPI_UID), TSPI_2_BASE, TSPI_2_CHN, &TSPI_2_DEV},
  {7, IP_DISC(TSPI_3_CHNS, TSPI_UID), TSPI_3_BASE, TSPI_3_CHN, &TSPI_3_DEV},
  {7, IP_DISC(TSPI_4_CHNS, TSPI_UID), TSPI_4_BASE, TSPI_4_CHN, &TSPI_4_DEV},

};
int fpgaIpGiidNum = (sizeof(fpgaIp)/sizeof(sFpgaIp));
