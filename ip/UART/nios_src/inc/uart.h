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
 * @file uart.h
 * @brief
 */

#ifndef UART_H_
#define UART_H_

#define UART_UID 0x01345

#define UART_IP_VER   0x0105
#define UART_DRV_VER  0x0004
#define UART_VER (((UART_IP_VER)<<16)|(UART_DRV_VER))

#define UART_0_CHNS 1
#define UART_1_CHNS 1
#define UART_2_CHNS 1
#define UART_3_CHNS 1
#define UART_4_CHNS 1
#define UART_5_CHNS 1
#define UART_6_CHNS 1
#define UART_7_CHNS 1

#define UART_PIN_FNC_TX  PIN_FNC(0x01, PIN_DIR_O)
#define UART_PIN_FNC_RX  PIN_FNC(0x02, PIN_DIR_I)
#define UART_PIN_FNC_RTS PIN_FNC(0x03, PIN_DIR_O)
#define UART_PIN_FNC_CTS PIN_FNC(0x04, PIN_DIR_I)
#define UART_PIN_FNC_DTR PIN_FNC(0x05, PIN_DIR_O)
#define UART_PIN_FNC_DSR PIN_FNC(0x06, PIN_DIR_I)

#include <arduino_16550_uart.h>

typedef struct uart_dev_s{
  char                     *name;
  arduino_16550_uart_state *sp;
} sUartDev, *psUartDev;

#define UART_DEV(name) sUartDev name##_DEV = {name##_NAME, 0}

void uartRpc(void);

#endif /* UART_H_ */
