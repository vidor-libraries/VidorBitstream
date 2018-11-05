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
  { 2, 19, 0, 0, 0},
  { 2, 20, 0, 0, 0},
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
 */
sFpgaIpRpc fpgaIpRpc[] = {
#if defined(NO_RPC) && (NO_RPC == 1)
#else
  {fpgaRpc, FPGA_VER    },
  {pioRpc , PIO_VER     },
  {pwmRpc , PWM_VER     },
};

sFpgaIpLoop fpgaIpLoop[] = {
};
int fpgaIpLoopNum = (sizeof(fpgaIpLoop)/sizeof(sFpgaIpLoop));

/* TODO va preso da system.h */
#define SAM_PIO_CHNS  23
#define PEX_PIO_CHNS  25
#define WM_PIO_CHNS   22

#define FIX_IO_CHNS    21 // TODO

#define SAM_PWM_CHNS  23

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
  {1, IP_DISC(SAM_PIO_CHNS, PIO_UID), SAM_PIO_BASE, 0},
  {1, IP_DISC(PEX_PIO_CHNS, PIO_UID), PEX_PIO_BASE, 0},
  {1, IP_DISC(WM_PIO_CHNS , PIO_UID), WM_PIO_BASE, 0},
  {1, IP_DISC(FIX_IO_CHNS , FIO_UID), NULL, 0},

  /**
   * PWM
   */
  {2, IP_DISC(SAM_PWM_CHNS, PWM_UID), SAM_PWM_BASE, PWM_0_CHN},

};
int fpgaIpGiidNum = (sizeof(fpgaIp)/sizeof(sFpgaIp));
