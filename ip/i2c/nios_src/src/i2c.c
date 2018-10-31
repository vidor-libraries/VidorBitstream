/*
* Copyright 2018 ARDUINO SA (http://www.arduino.cc/)
* This file is part of Vidor IP.
* Copyright (c) 2018
* Authors: Dario Pennisi
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
*/


#include <stdio.h>
#include <io.h>

#include "config.h"
#include "mb.h"
#include "rpc.h"
#include "i2c.h"

// Overall status and control
#define IOWR_I2C_PRERLO(port,data)  IOWR(port, 0, data)
#define IOWR_I2C_PRERHI(port,data)  IOWR(port, 1, data)
#define IOWR_I2C_CTR(port,data)     IOWR(port, 2, data)
#define IOWR_I2C_TXR(port,data)     IOWR(port, 3, data)
#define IOWR_I2C_CR(port,data)      IOWR(port, 4, data)
#define IORD_I2C_PRERLO(port)       IORD(port, 0)
#define IORD_I2C_PRERHI(port)       IORD(port, 1)
#define IORD_I2C_CTR(port)          IORD(port, 2)
#define IORD_I2C_RXR(port)          IORD(port, 3)
#define IORD_I2C_SR(port)           IORD(port, 4)

/* ----- Control register */
#define OC_I2C_EN (1<<7) /* Core enable bit core is: 1: enabled, 0: disabled */
#define OC_I2C_IEN (1<<6) /* Interrupt enable bit Interrupt is: 1: enabled, 0: disabled */
/* Other bits in CR are reserved */

/* ----- Command register bits */
#define I2C_CR_STA   0x80
#define I2C_CR_STO   0x40
#define I2C_CR_RD    0x20
#define I2C_CR_WR    0x10
#define I2C_CR_ACK   0x08
#define I2C_CR_IACK  0x01

/* ----- Status register bits */
#define OC_I2C_RXACK (1<<7) /* ACK received from slave  1: ACK, 0: NACK */
#define OC_I2C_BUSY  (1<<6) /* Busy bit */
#define OC_I2C_TIP   (1<<1) /* Transfer in progress */
#define OC_I2C_IF    (1<<0) /* Interrupt flag */

#define I2C_SR_TIP   0x02
#define I2C_SR_RXACK 0x80

void _i2c_wait_tip(alt_u32 base);

alt_u32 i2cSetup(alt_u32 cmd, alt_u32 baud);
alt_u32 i2cEnd(alt_u32 cmd);

alt_u32 i2cWrite(alt_u32 giid, alt_u8 address, alt_u8* data, alt_u32 len);
alt_u32 i2cRead(alt_u32 giid, alt_u8 address, alt_u8* data, alt_u32 len);

/**
 *
 */
void i2cRpc(void)
{
  alt_u32 volatile *rpc = mbPtrGet();
  alt_u32 ret;

  ret = -1;
  if ((fpgaIp[RPC_GIID(rpc[0])].disc & 0xFFFFF) != I2C_UID) {
    rpc[1] = ret;
    return ;
  }
  switch (RPC_PID(rpc[0])) {
  case 2:
    ret = i2cSetup(rpc[0], rpc[1]);
    break;
  case 4:
    ret = i2cEnd(rpc[0]);
    break;
  case 5:
    ret = i2cRead(RPC_GIID(rpc[0]), rpc[1], (alt_u8*)&rpc[3], rpc[2]);
    break;
  case 6:
    ret = i2cWrite(RPC_GIID(rpc[0]), rpc[1], (alt_u8*)&rpc[3], rpc[2]);
    break;
  }
  rpc[1] = ret;
}

/**
 */
alt_u32 i2cSetup(alt_u32 cmd, alt_u32 baud)
{
  alt_u32 baseaddr = (alt_u32)fpgaIp[RPC_GIID(cmd)].base;

  /* Setup prescaler for baud with sysclk of 154MHz */
  int prescale = 154000000/(5*baud);

  IOWR_I2C_CTR(baseaddr, 0x00); /* Disable core */

  IOWR_I2C_PRERLO(baseaddr,  prescale & 0xff);
  IOWR_I2C_PRERHI(baseaddr, (prescale & 0xff00)>>8);

  IOWR_I2C_CTR(baseaddr, OC_I2C_EN); /* Enable core */

  return 0;
}

/**
 */
alt_u32 i2cEnd(alt_u32 cmd)
{
  alt_u32 baseaddr = (alt_u32)fpgaIp[RPC_GIID(cmd)].base;

  /* disable core */
  IOWR_I2C_CTR(baseaddr, I2C_CR_STO);
  return 0;
}

/**
 *
 */
alt_u32 i2cRead(alt_u32 giid, alt_u8 address, alt_u8* data, alt_u32 len)
{
  alt_u32 baseaddr = (alt_u32)fpgaIp[giid].base;
  int i;

  _i2c_wait_tip(baseaddr);

  // write address
  IOWR_I2C_TXR(baseaddr, (address<<1)|1);
  IOWR_I2C_CR(baseaddr, I2C_CR_STA | I2C_CR_WR);
  _i2c_wait_tip(baseaddr);
  if(IORD_I2C_SR(baseaddr) & I2C_SR_RXACK){
    return -2;
  }

  // read data
  for(i=0; i<len; i++){
    if(i==(len-1)){
      IOWR_I2C_CR(baseaddr, I2C_CR_RD  | I2C_CR_ACK | I2C_CR_STO);
    }else{
      IOWR_I2C_CR(baseaddr, I2C_CR_RD);
    }
    _i2c_wait_tip(baseaddr);
    *(data + i) = IORD_I2C_RXR(baseaddr);
  }

  return 0;
}

/**
 *
 */
alt_u32 i2cWrite(alt_u32 giid, alt_u8 address, alt_u8* data, alt_u32 len)
{
  alt_u32 baseaddr = (alt_u32)fpgaIp[giid].base;
  alt_u32 i;

  _i2c_wait_tip(baseaddr);

  // write address
  IOWR_I2C_TXR(baseaddr, address<<1);
  IOWR_I2C_CR(baseaddr, I2C_CR_STA | I2C_CR_WR);
  _i2c_wait_tip(baseaddr);
  if(IORD_I2C_SR(baseaddr) & I2C_SR_RXACK){
    return -2;
  }

  for(i=0; i<len-1; i++){
    IOWR_I2C_TXR(baseaddr, data[i]);
    IOWR_I2C_CR(baseaddr, I2C_CR_WR);
    _i2c_wait_tip(baseaddr);
    if(IORD_I2C_SR(baseaddr) & I2C_SR_RXACK){
      return -2;
    }
  }
  IOWR_I2C_TXR(baseaddr, data[len-1]);
  IOWR_I2C_CR(baseaddr, I2C_CR_WR | I2C_CR_STO);
  _i2c_wait_tip(baseaddr);
  if(IORD_I2C_SR(baseaddr) & I2C_SR_RXACK){
    return -2;
  }

  return 0;
}

/**
 *
 */
void _i2c_wait_tip(alt_u32 base)
{
  while(IORD_I2C_SR(base) & I2C_SR_TIP);
}
