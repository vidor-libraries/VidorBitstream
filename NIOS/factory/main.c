/*
 * main.c
 *
 *  Created on: May 29, 2018
 *      Author: max
 */

#include <io.h>
#include <string.h>
#include <system.h>
#include <linker.h>

#include "gfx.h"
#include "sign.h"
#include "irq.h"
#include "platform.h"

//#define SF_TEST

#ifdef SF_TEST
#include "mb.h"
void sfTest(void)
{
  alt_u32 volatile *rpc = (alt_u32*)MB_BASE;

  // JedecId
  rpc[0] = MB_DEV_SF | 0x01;
  platformCmd();

  // UniqueId
  memset(rpc, 0, 32);
  rpc[0] = MB_DEV_SF | 0x02;
  platformCmd();

  // read
  memset(rpc, 0, 32);
  rpc[0] = MB_DEV_SF | 0x05;
  rpc[1] = 0;
  rpc[2] = 16;
  platformCmd();

  // read
  memset(rpc, 0, 32);
  rpc[0] = MB_DEV_SF | 0x05;
  rpc[1] = 64*1024;
  rpc[2] = 16;
  platformCmd();

  // erase first 64K
  rpc[0] = MB_DEV_SF | 0x03;
  rpc[1] = 2;
  rpc[2] = 0;
  platformCmd();

  // erase second 64K
  rpc[0] = MB_DEV_SF | 0x03;
  rpc[1] = 2;
  rpc[2] = 1;
  platformCmd();

  // program
  rpc[0] = MB_DEV_SF | 0x04;
  rpc[1] = 0;
  rpc[2] = 16;
  strcpy(&rpc[3], "01234567890abcdef");
  platformCmd();

  // program
  rpc[0] = MB_DEV_SF | 0x04;
  rpc[1] = 64*1024;
  rpc[2] = 16;
  strcpy(&rpc[3], "fedcba9876543210");
  platformCmd();

  // read
  memset(rpc, 0, 32);
  rpc[0] = MB_DEV_SF | 0x05;
  rpc[1] = 0;
  rpc[2] = 32;
  platformCmd();

  // read
  memset(rpc, 0, 32);
  rpc[0] = MB_DEV_SF | 0x05;
  rpc[1] = 64*1024;
  rpc[2] = 32;
  platformCmd();
}
#endif  //SF_TEST

extern GFXgc gfxDefaultGc;

/**
 *
 */
int main()
{
  int ret;

  // initial logo
  gfxInit(0);
  // black square
  fillRect(&gfxDefaultGc, 0, 0, 10, 10, 0x8000);

  ret = signGen();
  if (ret) {
    // red square
    fillRect(&gfxDefaultGc, 0, 0, 10, 10, 0xFC00);
  } else {
    // green square
    fillRect(&gfxDefaultGc, 0, 0, 10, 10, 0x83E0);
  }
#ifdef SF_TEST
  sfTest();
#endif
  // start command manager
  irqPinSet(0, platformCmd);
  intPinInit(1, 0);

  platformSetup();
  while (1) {
    platformLoop();
  };

  return 0;
}
