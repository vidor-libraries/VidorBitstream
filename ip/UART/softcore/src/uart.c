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
 * @file uart.c
 * @brief UART driver
 */


#define SEC_RAM __attribute__((__section__(".rwdata")))

#include <io.h>
#include <sys/alt_irq.h>
#include <arduino_16550_uart.h>

#include "config.h"
#include "mb.h"
#include "rpc.h"
#include "uart.h"

#define UART_PARITY_EVEN (0x1ul)
#define UART_PARITY_ODD  (0x2ul)
#define UART_PARITY_NONE (0x3ul)
#define UART_PARITY_MASK (0xFul)

#define UART_STOP_BIT_1    (0x10ul)
#define UART_STOP_BIT_1_5  (0x20ul)
#define UART_STOP_BIT_2    (0x30ul)
#define UART_STOP_BIT_MASK (0xF0ul)

#define UART_DATA_5    (0x100ul)
#define UART_DATA_6    (0x200ul)
#define UART_DATA_7    (0x300ul)
#define UART_DATA_8    (0x400ul)
#define UART_DATA_MASK (0xF00ul)

alt_u32 uartSetup(alt_u32 cmd);
alt_u32 uartEnd(alt_u32 cmd);

alt_u32 uartEnable(alt_u32 cmd, alt_u32 mode);
alt_u32 uartSet(alt_u32 cmd, alt_u32 baud, alt_u32 config);
alt_u32 uartDisable(alt_u32 cmd);
alt_u32 uartGet(alt_u32 cmd);
alt_u32 uartRead(alt_u32 cmd, alt_u8* buf, alt_u32 len);
alt_u32 uartAvail(alt_u32 cmd);
alt_u32 uartPut(alt_u32 cmd, alt_u8 data);
alt_u32 uartWrite(alt_u32 cmd, alt_u8* buf, alt_u32 len);
alt_u32 uartFlush(alt_u32 cmd);

void uartIsrRx(void* arg);

/**
 *
 */
void SEC_RAM uartRpc(void)
{
  alt_u32 volatile *rpc = mbPtrGet();
  alt_u32 ret;

  ret = -1;
  if ((fpgaIp[RPC_GIID(rpc[0])].disc & 0xFFFFF) != UART_UID) {
    rpc[1] = ret;
    return ;
  }
  switch (RPC_PID(rpc[0])) {
  case  2: ret = uartSetup(rpc[0]); break;
  case  4: ret = uartEnd(rpc[0]); break;

  case  5: ret = uartSet(rpc[0], rpc[1], rpc[2]); break;
  case  6: ret = uartGet(rpc[0]); break;
  case  7: ret = uartRead(rpc[0], (alt_u8*)&rpc[2], rpc[1]); break;
  case  8: ret = uartAvail(rpc[0]); break;
  case  9: ret = uartPut(rpc[0], rpc[1]); break;
  case 10: ret = uartWrite(rpc[0], (alt_u8*)&rpc[2], rpc[1]); break;
  case 11: ret = uartFlush(rpc[0]); break;
  }
  rpc[1] = ret;
}

/**
 */
alt_u32 uartSetup(alt_u32 cmd)
{
  alt_u8    giid = RPC_GIID(cmd);
  psUartDev pDev = (psUartDev)fpgaIp[giid].priv;

  pDev->sp = arduino_16550_uart_open(pDev->name);
  if (!pDev->sp) {
    return -1;
  }
  arduino_16550_uart_isrrx(uartIsrRx, (void*)(cmd & 0xFFFFF000));
  return 0;
}

/**
 */
alt_u32 uartEnd(alt_u32 cmd)
{
  alt_u8    giid = RPC_GIID(cmd);
  psUartDev pDev = (psUartDev)fpgaIp[giid].priv;

  if (!pDev->sp) {
    return -1;
  }
  arduino_16550_uart_isrrx(NULL, NULL);
  return arduino_16550_uart_close(pDev->sp, 0);
}

/**
 * Config
 * StopBit   STOPB_1 = 0,STOPB_2
 * ParityBit ODD_PARITY = 0, EVEN_PARITY, MARK_PARITY, SPACE_PARITY, NO_PARITY
 * DataBit   CS_5 = 0, CS_6, CS_7, CS_8, CS_9 = 256
 *
 */
alt_u32 uartSet(alt_u32 cmd, alt_u32 baud, alt_u32 config)
{
  alt_u8      giid = RPC_GIID(cmd);
  psUartDev   pDev = (psUartDev)fpgaIp[giid].priv;
  UartConfig  setting;
  StopBit     sb;
  ParityBit   pb;
  DataBit     db;
  Baud        br;

  if (!pDev->sp) {
    return -1;
  }

  switch (baud) {
  case   9600: br =   BR9600; break;
  case  19200: br =  BR19200; break;
  case  38400: br =  BR38400; break;
  case  57600: br =  BR57600; break;
  case 115200: br = BR115200; break;
  default: br = baud; break;
  }
  switch (config & UART_STOP_BIT_MASK) {
  case UART_STOP_BIT_1  : sb = STOPB_1; break;
  case UART_STOP_BIT_1_5: return -1;
  case UART_STOP_BIT_2  : sb = STOPB_2; break;
  default: return -1;
  }
  switch (config & UART_PARITY_MASK) {
  case UART_PARITY_NONE: pb = NO_PARITY  ; break;
  case UART_PARITY_EVEN: pb = EVEN_PARITY; break;
  case UART_PARITY_ODD : pb = ODD_PARITY ; break;
  default: return -1;
  }
  switch (config & UART_DATA_MASK) {
  case UART_DATA_5: db = CS_5; break;
  case UART_DATA_6: db = CS_6; break;
  case UART_DATA_7: db = CS_7; break;
  case UART_DATA_8: db = CS_8; break;
  default: return -1;
  }

  setting.stop_bit      = sb;
  setting.parity_bit    = pb;
  setting.data_bit      = db;
  setting.baudrate      = br;
  setting.fifo_mode     = 0;
  setting.rx_fifo_level = 0;
  setting.tx_fifo_level = 0;
  setting.hwfc          = 0;

  return alt_16550_uart_config(pDev->sp, &setting);
}

/**
 *
 */
alt_u32 SEC_RAM uartGet(alt_u32 cmd)
{
  alt_u8    giid = RPC_GIID(cmd);
  psUartDev pDev = (psUartDev)fpgaIp[giid].priv;
  alt_u8    c;
  int       ret;

  if (!pDev->sp) {
    return -1;
  }
  ret = arduino_16550_uart_read(pDev->sp, &c, 1, 0);
  if (ret != 1) {
    return -1;
  }
  return c;
}

/**
 *
 */
alt_u32 SEC_RAM uartRead(alt_u32 cmd, alt_u8* buf, alt_u32 len)
{
  alt_u8    giid = RPC_GIID(cmd);
  psUartDev pDev = (psUartDev)fpgaIp[giid].priv;

  if (!pDev->sp) {
    return -1;
  }

  int       ret;
  alt_u8    b[ALT_16550_UART_BUF_LEN];
  alt_u32  *ps;
  alt_u32  *pd;
  int       i;

  ret = arduino_16550_uart_read(pDev->sp, b, len, 0);
  ps = (alt_u32*)b;
  pd = (alt_u32*)buf;
  for(i=0; i<(len+3)/4; i++){
    pd[i] = ps[i];
  }
  return ret;
//  return arduino_16550_uart_read(pDev->sp, buf, len, 0);
}

/**
 *
 */
alt_u32 SEC_RAM uartAvail(alt_u32 cmd)
{
  alt_u8    giid = RPC_GIID(cmd);
  psUartDev pDev = (psUartDev)fpgaIp[giid].priv;
  alt_u32   a;

  if (!pDev->sp) {
    return -1;
  }
  a = ALT_16550_UART_BUF_LEN + pDev->sp->rx_end - pDev->sp->rx_start;
  a &= ALT_16550_UART_BUF_MSK;
  return a;
}

/**
 *
 */
alt_u32 SEC_RAM uartPut(alt_u32 cmd, alt_u8 data)
{
  alt_u8    giid = RPC_GIID(cmd);
  psUartDev pDev = (psUartDev)fpgaIp[giid].priv;

  if (!pDev->sp) {
    return -1;
  }
  return arduino_16550_uart_write(pDev->sp, &data, 1, 0);
}

/**
 *
 */
alt_u32 SEC_RAM uartWrite(alt_u32 cmd, alt_u8* buf, alt_u32 len)
{
  alt_u8    giid = RPC_GIID(cmd);
  psUartDev pDev = (psUartDev)fpgaIp[giid].priv;

  if (!pDev->sp) {
    return -1;
  }

  alt_u8    b[ALT_16550_UART_BUF_LEN];
  alt_u32  *ps;
  alt_u32  *pd;
  int       i;

  ps = (alt_u32*)buf;
  pd = (alt_u32*)b;
  for(i=0; i<(len+3)/4; i++){
    pd[i] = ps[i];
  }

  return arduino_16550_uart_write(pDev->sp, b, len, 0);
}

/**
 *
 */
alt_u32 uartFlush(alt_u32 cmd)
{
  alt_u8    giid = RPC_GIID(cmd);
  psUartDev pDev = (psUartDev)fpgaIp[giid].priv;

  if (!pDev->sp) {
    return -1;
  }
  arduino_16550_uart_flush(pDev->sp);
  return 0;
}

/**
 *
 */
void uartIsrRx(void* arg)
{
  mbEveTx((alt_u32)arg);
}
