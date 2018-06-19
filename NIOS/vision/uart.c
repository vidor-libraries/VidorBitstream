/*
 * uart.c
 *
 *  Created on: May 8, 2018
 *      Author: max
 */

#include <system.h>
#include <altera_up_avalon_rs232.h>

#include "mb.h"

typedef struct {
  char             *name;
  alt_up_rs232_dev *dev;
}sUartDev, *psUartDev;

sUartDev uart_dev[] = {
  {NINA_UART_NAME, NULL}
};

alt_u32 uart_dev_num;

alt_u32 uartEnable(alt_u32 index);
alt_u32 uartSet(alt_u32 index, alt_u32 baud, alt_u32 config);
alt_u32 uartDisable(alt_u32 index);
alt_u32 uartGet(alt_u32 index);
alt_u32 uartRead(alt_u32 index, alt_u8* buf, alt_u32 len);
alt_u32 uartAvail(alt_u32 index);
alt_u32 uartPut(alt_u32 index, alt_u8 data);
alt_u32 uartWrite(alt_u32 index, alt_u8* buf, alt_u32 len);
alt_u32 uartFlush(alt_u32 index);

#if 0
#define ALTERA_UP_AVALON_RS232_INIT(name, device)  \
  { \
    /* make the device available to the system */ \
    alt_dev_reg(&device.dev); \
  }
#endif


/**
 *
 */
void uartInit(int devs)
{
  int i;

  uart_dev_num = devs;
  for(i=0; i<uart_dev_num; i++){
    uart_dev[i].dev = alt_up_rs232_open_dev(uart_dev[i].name);
    if (uart_dev[i].dev == NULL) {
      //return -1;
    }
  }
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
    ret = uartEnable(MB_SUB(rpc[0]));
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
alt_u32 uartEnable(alt_u32 index)
{
  return 0;
}

/**
 *
 */
alt_u32 uartSet(alt_u32 index, alt_u32 baud, alt_u32 config)
{
  return 0;
}

/**
 *
 */
alt_u32 uartDisable(alt_u32 index)
{
  return 0;
}

/**
 *
 */
alt_u32 uartGet(alt_u32 index)
{
  if(index >= uart_dev_num){
    return -1;
  }
  alt_up_rs232_dev *dev = uart_dev[index].dev;
  unsigned avail;
  alt_u8 data, parity;
  int status;

  avail = alt_up_rs232_get_used_space_in_read_FIFO(dev);
  if (!avail) {
    return -1;
  }
  status = alt_up_rs232_read_data(dev, &data, &parity);
  if (status) {
    return -1;
  }
  return data;
}

/**
 *
 */
alt_u32 uartRead(alt_u32 index, alt_u8* buf, alt_u32 len)
{
  if(index >= uart_dev_num){
    return -1;
  }
  alt_up_rs232_dev *dev = uart_dev[index].dev;
  unsigned avail;
  alt_u8 data, parity;
  int i;
  int status;

  for(i=0; i<len; i++){
    avail = alt_up_rs232_get_used_space_in_read_FIFO(dev);
    if (!avail) {
      break;
    }
    status = alt_up_rs232_read_data(dev, &data, &parity);
    if (status) {
      return -1;
    }
    buf[i] = data;
  }
  return i;
}

/**
 *
 */
alt_u32 uartAvail(alt_u32 index)
{
  if(index >= uart_dev_num){
    return -1;
  }
  alt_up_rs232_dev *dev = uart_dev[index].dev;
  return alt_up_rs232_get_used_space_in_read_FIFO(dev);
}

/**
 *
 */
alt_u32 uartPut(alt_u32 index, alt_u8 data)
{
  if(index >= uart_dev_num){
    return -1;
  }
  alt_up_rs232_dev *dev = uart_dev[index].dev;
  unsigned avail;
  int status;

  do {
    avail = alt_up_rs232_get_available_space_in_write_FIFO(dev);
  } while(avail == 0);
  status = alt_up_rs232_write_data(dev, data);
  if (status) {
    return -1;
  }
  return 0;
}

/**
 *
 */
alt_u32 uartWrite(alt_u32 index, alt_u8* buf, alt_u32 len)
{
  if(index >= uart_dev_num){
    return -1;
  }
  alt_up_rs232_dev *dev = uart_dev[index].dev;
  unsigned avail;
  int i;
  int status;

  for(i=0; i<len; i++){
    do {
      avail = alt_up_rs232_get_available_space_in_write_FIFO(dev);
    } while(avail == 0);

    status = alt_up_rs232_write_data(dev, buf[i]);
    if (status) {
      return -1;
    }
  }
  return i;
}

/**
 *
 */
alt_u32 uartFlush(alt_u32 index)
{
  return 0;
}
