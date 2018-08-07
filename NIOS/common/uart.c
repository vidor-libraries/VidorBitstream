/*
 * uart.c
 *
 *  Created on: May 8, 2018
 *      Author: max
 */
#include "config.h"

#if defined(UART_MODULE) && (UART_MODULE == 1)

#include <iptronix_16550_uart.h>

#include "mb.h"
#include "gpio.h"
#include "uart.h"

alt_u32 uart_dev_num;

typedef struct {
  char                     *name;
  alt_u32                   cap;
  alt_u32                   tx;
  alt_u32                   rx;
  alt_u32                   rts;
  alt_u32                   cts;
  alt_u32                   dtr;
  alt_u32                   dsr;
  alt_u32                   mode;
  altera_16550_uart_state  *sp;
}sUartDev, *psUartDev;

sUartDev uart_dev[] = {
  UART_DEV_CFG
};

/**
 *
 */
void uartInit(int devs)
{
  uart_dev_num = devs;
}

/**
 *
 */
void uartCmd(void)
{
  alt_u32 volatile *rpc = (alt_u32*)MB_BASE;
  alt_u32 ret = -1;

  switch(MB_CMD(rpc[0])){
  case 1:
    ret = uartEnable(MB_SUB(rpc[0]), rpc[1]);
    break;
  case 2:
    ret = uartSet(MB_SUB(rpc[0]), rpc[1], rpc[2]);
    break;
  case 3:
    ret = uartDisable(MB_SUB(rpc[0]));
    break;
  case 4:
    ret = uartGet(MB_SUB(rpc[0]));
    break;
  case 5:
    ret = uartRead(MB_SUB(rpc[0]), (alt_u8*)&rpc[2], rpc[1]);
    break;
  case 6:
    ret = uartAvail(MB_SUB(rpc[0]));
    break;
  case 7:
    ret = uartPut(MB_SUB(rpc[0]), rpc[1]);
    break;
  case 8:
    ret = uartWrite(MB_SUB(rpc[0]), (alt_u8*)&rpc[2], rpc[1]);
    break;
  case 9:
    ret = uartFlush(MB_SUB(rpc[0]));
    break;
  }
  rpc[1] = ret;
}

/**
 *
 */
alt_u32 uartEnable(alt_u32 idx, alt_u32 mode)
{
  if(idx >= uart_dev_num){
    return -1;
  }
  psUartDev pDev = &uart_dev[idx];

  if ((mode & pDev->cap) != mode) {
    return -1;
  }
  pDev->mode = mode;

  pDev->sp = altera_16550_uart_open(pDev->name);
  if (!pDev->sp) {
    return -1;
  }

  gpioPinMode(pDev->tx, UART_PIN_MUX);
  gpioPinMode(pDev->rx, 0);
  if (mode & 0x1) {
    gpioPinMode(pDev->rts, UART_PIN_MUX);
    gpioPinMode(pDev->cts, 0);
  }
  if (mode & 0x2) {
    gpioPinMode(pDev->dtr, UART_PIN_MUX);
    gpioPinMode(pDev->dsr, 0);
  }

  return 0;
}

/**
 * Config
 * StopBit   STOPB_1 = 0,STOPB_2
 * ParityBit ODD_PARITY = 0, EVEN_PARITY, MARK_PARITY, SPACE_PARITY, NO_PARITY
 * DataBit   CS_5 = 0, CS_6, CS_7, CS_8, CS_9 = 256
 *
 */
alt_u32 uartSet(alt_u32 idx, alt_u32 baud, alt_u32 config)
{
  if(idx >= uart_dev_num){
    return -1;
  }
  psUartDev   pDev = &uart_dev[idx];
  UartConfig  setting;
  StopBit     sb;
  ParityBit   pb;
  DataBit     db;
  Baud        br;

  switch (baud) {
  case   9600: br =   BR9600; break;
  case  19200: br =  BR19200; break;
  case  38400: br =  BR38400; break;
  case  57600: br =  BR57600; break;
  case 115200: br = BR115200; break;
  default: return -1;;
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

/* TODO remove
  sb = STOPB_1;
  pb = NO_PARITY;
  db = CS_8;
 remove */

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
alt_u32 uartDisable(alt_u32 idx)
{
  if(idx >= uart_dev_num){
    return -1;
  }
  psUartDev   pDev = &uart_dev[idx];
  return altera_16550_uart_close(pDev->sp, 0); // TODO flags;
}

/**
 *
 */
alt_u32 uartGet(alt_u32 idx)
{
  if(idx >= uart_dev_num){
    return -1;
  }
  psUartDev pDev = &uart_dev[idx];
  alt_u8    c;
  int       ret;

  ret = altera_16550_uart_read(pDev->sp, &c, 1, 0);
  if (ret != 1) {
    return -1;
  }
  return c;
}

/**
 *
 */
alt_u32 uartRead(alt_u32 idx, alt_u8* buf, alt_u32 len)
{
  if(idx >= uart_dev_num){
    return -1;
  }
  psUartDev   pDev = &uart_dev[idx];
  return altera_16550_uart_read(pDev->sp, buf, len, 0);
}

/**
 *
 */
alt_u32 uartAvail(alt_u32 idx)
{
  if(idx >= uart_dev_num){
    return -1;
  }
  psUartDev   pDev = &uart_dev[idx];
  alt_u32 a;

  a = ALT_16550_UART_BUF_LEN + pDev->sp->rx_end - pDev->sp->rx_start;
  a &= ALT_16550_UART_BUF_MSK;
  return a;
}

/**
 *
 */
alt_u32 uartPut(alt_u32 idx, alt_u8 data)
{
  if(idx >= uart_dev_num){
    return -1;
  }
  psUartDev   pDev = &uart_dev[idx];
  return altera_16550_uart_write(pDev->sp, &data, 1, 0);
}

/**
 *
 */
alt_u32 uartWrite(alt_u32 idx, alt_u8* buf, alt_u32 len)
{
  if(idx >= uart_dev_num){
    return -1;
  }
  psUartDev   pDev = &uart_dev[idx];
  return altera_16550_uart_write(pDev->sp, buf, len, 0);
}

/**
 *
 */
alt_u32 uartFlush(alt_u32 idx)
{
  if(idx >= uart_dev_num){
    return -1;
  }
  psUartDev   pDev = &uart_dev[idx];
  altera_16550_uart_flush(pDev->sp);
  return 0;
}

#endif /* defined(UART_MODULE) && (UART_MODULE == 1) */
