// ********************************************************************************
// DisplayPort Core test code I2C handling
//
// All rights reserved. Property of Bitec.
// Restricted rights to use, duplicate or disclose this code are
// granted through contract.
//
// (C) Copyright Bitec 2012
//    All rights reserved
//
// Author         : $Author: swbranch $ @ bitec-dsp.com
// Department     :
// Date           : $Date: 2016/02/08 $
// Revision       : $Revision: #1 $
// URL            : $URL: svn://nas-bitec-fi/dp/trunk/software/dp_demo/i2c.c $
//
// Description:
//
// ********************************************************************************

#include <stdio.h>
#include <io.h>
#include "i2c.h"

#define DEBUG_PRINT_ENABLED 0
#if DEBUG_PRINT_ENABLED
#define DGB_PRINTF printf
#else
#define DGB_PRINTF(format, args...) ((void)0)
#endif

// Overall status and control
#define IOWR_I2C_PRERLO(port,data)  IOWR(port, 0, data)
#define IOWR_I2C_PRERHI(port,data)  IOWR(port, 1, data)
#define IOWR_I2C_CTR(port,data)     IOWR(port, 2, data)
#define IOWR_I2C_TXR(port,data)     IOWR(port, 3, data)
#define IOWR_I2C_CR(port,data)      IOWR(port, 4, data)
#define IORD_I2C_PRERLO             IORD(bitec_i2c_baseaddr, 0)
#define IORD_I2C_PRERHI             IORD(bitec_i2c_baseaddr, 1)
#define IORD_I2C_CTR                IORD(bitec_i2c_baseaddr, 2)
#define IORD_I2C_RXR                IORD(bitec_i2c_baseaddr, 3)
#define IORD_I2C_SR                 IORD(bitec_i2c_baseaddr, 4)

#define I2C_CR_STA 0x80
#define I2C_CR_STO 0x40
#define I2C_CR_RD 0x20
#define I2C_CR_WR 0x10
#define I2C_CR_ACK 0x08
#define I2C_CR_IACK 0x01

#define I2C_SR_TIP 0x2
#define I2C_SR_RXACK 0x80

unsigned int bitec_i2c_baseaddr;

void _bitec_i2c_wait_tip();

void bitec_i2c_init(unsigned int baseaddr)
{
  bitec_i2c_baseaddr = baseaddr;

  // Setup prescaler for 100KHz with sysclk of 154MHz
  int prescale = 154000000/(5*100000);
  IOWR_I2C_PRERLO(bitec_i2c_baseaddr, prescale & 0xff);
  IOWR_I2C_PRERHI(bitec_i2c_baseaddr, (prescale & 0xff00)>>8);
  // Enable core
  IOWR_I2C_CTR(bitec_i2c_baseaddr, 0x80);
}

void bitec_i2c_read(unsigned char address, unsigned char *data, unsigned char len)
{
    unsigned int i;

    _bitec_i2c_wait_tip(bitec_i2c_baseaddr);
  
   // write address
  IOWR_I2C_TXR(bitec_i2c_baseaddr, address);
  IOWR_I2C_CR(bitec_i2c_baseaddr, I2C_CR_STA | I2C_CR_WR);
  _bitec_i2c_wait_tip(bitec_i2c_baseaddr);
  if(IORD_I2C_SR & I2C_SR_RXACK)
  {
    DGB_PRINTF("bitec_i2c_read: Slave replied NACK to address\n");
    return;
  }

  // read data
  for(i=0;i<len;i++)
  {
    if(i==(len-1))
      IOWR_I2C_CR(bitec_i2c_baseaddr, I2C_CR_RD  | I2C_CR_ACK | I2C_CR_STO);
    else
      IOWR_I2C_CR(bitec_i2c_baseaddr, I2C_CR_RD);

    _bitec_i2c_wait_tip(bitec_i2c_baseaddr);
    *(data + i) = IORD_I2C_RXR;
    DGB_PRINTF("Byte %X = %X\n",i,IORD_I2C_RXR);
  }

}

void bitec_i2c_write(unsigned char address, unsigned char reg, unsigned char data)
{
  _bitec_i2c_wait_tip(bitec_i2c_baseaddr);

  // write address
  IOWR_I2C_TXR(bitec_i2c_baseaddr, address);
  IOWR_I2C_CR(bitec_i2c_baseaddr, I2C_CR_STA | I2C_CR_WR);
  _bitec_i2c_wait_tip(bitec_i2c_baseaddr);
  if(IORD_I2C_SR & I2C_SR_RXACK)
  {
    DGB_PRINTF("bitec_i2c_write: Slave replied NACK to address\n");
    return;
  }

  // write register address
  IOWR_I2C_TXR(bitec_i2c_baseaddr, reg);
  IOWR_I2C_CR(bitec_i2c_baseaddr, I2C_CR_WR);
  _bitec_i2c_wait_tip(bitec_i2c_baseaddr);
  if(IORD_I2C_SR & I2C_SR_RXACK)
  {
    DGB_PRINTF("bitec_i2c_write: Slave replied NACK to reg\n");
    return;
  }

  // write data
  IOWR_I2C_TXR(bitec_i2c_baseaddr, data);
  IOWR_I2C_CR(bitec_i2c_baseaddr, I2C_CR_WR | I2C_CR_STO);
  _bitec_i2c_wait_tip(bitec_i2c_baseaddr);
  if(IORD_I2C_SR & I2C_SR_RXACK)
  {
    DGB_PRINTF("bitec_i2c_write: Slave replied NACK to data\n");
    return;
  }
}

void bitec_i2c_writeaddr(unsigned char address, unsigned char reg)
{
  _bitec_i2c_wait_tip(bitec_i2c_baseaddr);

  // write address
  IOWR_I2C_TXR(bitec_i2c_baseaddr, address);
  IOWR_I2C_CR(bitec_i2c_baseaddr, I2C_CR_STA | I2C_CR_WR);
  _bitec_i2c_wait_tip(bitec_i2c_baseaddr);
  if(IORD_I2C_SR & I2C_SR_RXACK)
  {
    DGB_PRINTF("bitec_i2c_write: Slave replied NACK to address\n");
    return;
  }

  // write register address
  IOWR_I2C_TXR(bitec_i2c_baseaddr, reg);
  IOWR_I2C_CR(bitec_i2c_baseaddr, I2C_CR_WR| I2C_CR_STO);
  _bitec_i2c_wait_tip(bitec_i2c_baseaddr);
  if(IORD_I2C_SR & I2C_SR_RXACK)
  {
    DGB_PRINTF("bitec_i2c_write: Slave replied NACK to reg\n");
    return;
  }
}


void _bitec_i2c_wait_tip()
{
  while((IORD_I2C_SR & I2C_SR_TIP) > 0)
  {}
}
