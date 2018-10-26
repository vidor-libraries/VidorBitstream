/*
 * pio.c
 *
 *  Created on: May 7, 2018
 *      Author: max
 */


#include <stdio.h>
#include <io.h>

#include <system.h>

#include "config.h"
#include "mb.h"
#include "rpc.h"
#include "pio.h"

/* PIO registers */
#define PIO_VAL   0x00
#define PIO_DIR   0x01
#define PIO_CLR   0x02
#define PIO_SET   0x03
#define PIO_MUX0  0x04
#define PIO_MUX1  0x05

alt_u32 pioSetup(alt_u32 cmd);
alt_u32 pioEnd(alt_u32 cmd);

alt_u32 pioPinMode(alt_u32 cmd, alt_u8 mux, alt_u8 dir, alt_u8 val);
alt_u32 pioWrite(alt_u32 cmd, alt_u32 val);
alt_u32 pioRead(alt_u32 cmd);


/**
 *
 */
void pioRpc(void)
{
  alt_u32 volatile *rpc = mbPtrGet();
  alt_u32 ret;

  ret = -1;
  if ((fpgaIp[RPC_GIID(rpc[0])].disc & 0xFFFFF) != PIO_UID) {
    rpc[1] = ret;
    return ;
  }
  switch (RPC_PID(rpc[0])) {
  case 2: ret = pioSetup(rpc[0]); break;
  case 4: ret = pioEnd(rpc[0]); break;

  case 5: ret = pioPinMode(rpc[0], rpc[1], rpc[2], rpc[3]); break;
  case 6: ret = pioWrite(rpc[0], rpc[1]); break;
  case 7: ret = pioRead(rpc[0]); break;
  }
  rpc[1] = ret;
}

/**
 *
 */
alt_u32 pioSetup(alt_u32 cmd)
{
  return 0;
}

/**
 *
 */
alt_u32 pioEnd(alt_u32 cmd)
{
  return 0;
}

/**
 */
alt_u32 pioMode(alt_u8 port, alt_u8 pin, alt_u8 mux, alt_u8 dir, alt_u8 val)
{
  alt_u32 base;
  alt_u32 reg;

  base = fpgaIp[port+1].base;

  // set direction
  reg = IORD(base, PIO_DIR);
  if (dir){
    reg |= 1 << pin;
    // set value
    if(val){
      IOWR(base, PIO_SET, 1 << pin);
    }else{
      IOWR(base, PIO_CLR, 1 << pin);
    }
  }else{
    reg &=~ 1 << pin;
  }
  IOWR(base, PIO_DIR, reg);

  // set MUX
  reg = IORD(base, PIO_MUX0 + (pin >> 4));
  reg &= ~(0x3 << ((pin & 0x0F) << 1));
  reg |= (mux & 0x03) << ((pin & 0x0F) << 1);
  IOWR(base, PIO_MUX0 + (pin >> 4), reg);

  return 0;
}

/**
 */
alt_u32 pioPinMode(alt_u32 cmd, alt_u8 mux, alt_u8 dir, alt_u8 val)
{
  return pioMode(RPC_GIID(cmd)-1, RPC_CHN(cmd), mux, dir, val);
}

/**
 *
 */
alt_u32 pioWrite(alt_u32 cmd, alt_u32 val)
{
  alt_u8  giid = RPC_GIID(cmd);
  alt_u16 chn  = RPC_CHN(cmd);
  alt_u32 base = fpgaIp[giid].base;

  if(val){
    IOWR(base, PIO_SET, 1 << chn);
  }else{
    IOWR(base, PIO_CLR, 1 << chn);
  }
  return 0;
}

/**
 *
 */
alt_u32 pioRead(alt_u32 cmd)
{
  alt_u8  giid = RPC_GIID(cmd);
  alt_u16 chn  = RPC_CHN(cmd);
  alt_u32 base = fpgaIp[giid].base;
  alt_u32 val;

  val = IORD(base, PIO_VAL) & (1 << chn);
  return val? 1: 0;
}
