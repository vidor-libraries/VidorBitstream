/*
 * spi.c
 *
 *  Created on: Apr 18, 2018
 *      Author: max
 *
 * opencore tiny spi
 */

#include "config.h"

#if defined(SPI_MODULE) && (SPI_MODULE == 1)

#include <io.h>

#include "mb.h"
#include "gpio.h"

#define SPI_RXDATA    0
#define SPI_TXDATA    1
#define SPI_STATUS    2
#define SPI_CONTROL   3
#define SPI_BAUD      4

#define SPI_STATUS_TXE 0x1
#define SPI_STATUS_TXR 0x2

#define SPI_SS_H    0x04
#define SPI_SS_L    0x00

#define SPI_CPHA    0x01
#define SPI_CPOL    0x02

#define SPI_MODE_0    (0|0)
#define SPI_MODE_1    (0|SPI_CPHA)
#define SPI_MODE_2    (SPI_CPOL|0)
#define SPI_MODE_3    (SPI_CPOL|SPI_CPHA)

typedef struct {
  alt_u32   base;
  alt_u32   mosi;
  alt_u32   miso;
  alt_u32   clk;
  alt_u32   ss;
  alt_u32   ss_auto;
  alt_u32   mode;
}sSpiDev, *psSpiDev;

sSpiDev spi_dev[] = {
  SPI_DEV_CFG
};

alt_u32 spi_dev_num;

alt_u32 spiEnable(alt_u32 idx);
alt_u32 spiModeSet(alt_u32 idx, alt_u32 baud, alt_u32 mode, alt_u32 bitOrder);
alt_u32 spiDisable(alt_u32 idx);
alt_u32 spiTrx(alt_u32 idx, alt_u8* buf, alt_u32 len);

/**
 *
 */
void spiInit(int devs)
{
  spi_dev_num = devs;
}

/**
 *
 */
void spiCmd(void)
{
  alt_u32 volatile *rpc = (alt_u32*)MB_BASE;
  alt_u32 ret;

  ret = -1;
  switch(MB_CMD(rpc[0])){
  case 1:
    ret = spiEnable(MB_SUB(rpc[0]));
    break;
  case 2:
    ret = spiModeSet(MB_SUB(rpc[0]), rpc[1], rpc[2], rpc[3]);
    break;
  case 3:
    ret = spiDisable(MB_SUB(rpc[0]));
    break;
  case 4:
    ret = spiTrx(MB_SUB(rpc[0]), (alt_u8*)&rpc[2], rpc[1]);
    break;
  case 5:
    spiTrx(MB_SUB(rpc[0]), (alt_u8*)&rpc[1], 1);
    ret = rpc[1];
    break;
  }
  rpc[1] = ret;
}

/**
 */
alt_u32 spiEnable(alt_u32 idx)
{
  if(idx >= (sizeof(spi_dev)/sizeof(sSpiDev))){
    return -1;
  }
  psSpiDev pDev = &spi_dev[idx];
  /* gpio mux */
  if (pDev->mosi != -1) {
    gpioPinMode(pDev->mosi, 5); // MOSI
    gpioPinMode(pDev->miso, 0); // MISO
    gpioPinMode(pDev->clk , 5); // CLK
    gpioPinMode(pDev->ss  , 5); // SS
    IOWR(pDev->base, SPI_CONTROL, SPI_SS_H);
  }
  return 0;
}

/**
 */
alt_u32 spiModeSet(alt_u32 idx, alt_u32 baud, alt_u32 mode, alt_u32 bitOrder)
{
  if(idx >= (sizeof(spi_dev)/sizeof(sSpiDev))){
    return -1;
  }
  psSpiDev pDev = &spi_dev[idx];

  switch (mode) {
  case 0: pDev->mode = SPI_MODE_0; break;
  case 1: pDev->mode = SPI_MODE_1; break;
  case 2: pDev->mode = SPI_MODE_2; break;
  case 3: pDev->mode = SPI_MODE_3; break;
  default: return -1;
  }
  IOWR(pDev->base, SPI_CONTROL, SPI_SS_H | pDev->mode);
  IOWR(pDev->base, SPI_BAUD, ALT_CPU_FREQ / (baud*2));

  return 0;
}

/**
 */
alt_u32 spiDisable(alt_u32 idx)
{
  if(idx >= (sizeof(spi_dev)/sizeof(sSpiDev))){
    return -1;
  }
//  psSpiDev pDev = &spi_dev[idx];
  return 0;
}

/**
 */
alt_u32 spiTrx(alt_u32 idx, alt_u8* buf, alt_u32 len)
{
  if(idx >= spi_dev_num){
    return -1;
  }
  psSpiDev  pDev = &spi_dev[idx];
  alt_u32   base = pDev->base;
  alt_u8   *rxb = buf;

  /* set SS */
  if (pDev->ss_auto) {
    IOWR(base, SPI_CONTROL, SPI_SS_L | pDev->mode);
  }

  IOWR(base, SPI_TXDATA, *buf++);
  len--;
  if (len) {
    IOWR(base, SPI_TXDATA, *buf++);
  }
  while (len--) {
    while (!(IORD(base, SPI_STATUS) & SPI_STATUS_TXR));
    IOWR(base, SPI_TXDATA, *buf++);
    *rxb++ = IORD(base, SPI_RXDATA);
  }
  while (!(IORD(base, SPI_STATUS) & SPI_STATUS_TXE));
  *rxb++ = IORD(base, SPI_RXDATA);

  /* clear SS */
  if (pDev->ss_auto) {
    IOWR(base, SPI_CONTROL, SPI_SS_H | pDev->mode);
  }
  return 0;
}

/**
 */
alt_u32 spiTrc(alt_u32 idx, alt_u32 txl, alt_u8* txb, alt_u32 rxl, alt_u8* rxb)
{
  if(idx >= (sizeof(spi_dev)/sizeof(sSpiDev))){
    return -1;
  }
  psSpiDev  pDev = &spi_dev[idx];
  alt_u32   base = pDev->base;
  int       i;
  alt_u32   bytes = txl+rxl;
  alt_u8    rx, tx;
  int       fr;

  /* set SS */
  if (pDev->ss_auto) {
    IOWR(base, SPI_CONTROL, SPI_SS_L | pDev->mode);
  }

  for (i=0;i<bytes;i++) {
    if (i<txl)
      IOWR(base, SPI_TXDATA, *txb++);
    else
      IOWR(base, SPI_TXDATA, 0xff);
    if (i>0) {
      while (!(IORD(base, SPI_STATUS) & SPI_STATUS_TXR));
      if (i>txl)
    	*rxb++ = IORD(base, SPI_TXDATA);
    }
  }
  while (!(IORD(base, SPI_STATUS) & SPI_STATUS_TXE));
  if (rxl)
  	*rxb++ = IORD(base, SPI_RXDATA);

  /* clear SS */
  if (pDev->ss_auto) {
    IOWR(base, SPI_CONTROL, SPI_SS_H | pDev->mode);
  }
  return 0;
}

#endif /* defined(SPI_MODULE) && (SPI_MODULE == 1) */
