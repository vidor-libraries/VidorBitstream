/*
 * platform.c
 *
 *  Created on: May 9, 2018
 *      Author: max
 */
#include <stdio.h>
#include <nios2.h>
#include <system.h>

#include "config.h"

#include "mb.h"
#include "irq.h"
#include "sf.h"
#include "ru.h"

/**
 */
typedef struct {
  void(*setup)(int);
  void(*cmd)(void);
  void(*loop)(void);
  alt_u32 dev_cod;
  alt_u32 sub_devs;
}sDevHnd, *psDevHnd;

/**
 *
 */
void pltCmd(void);

/**
 *
 */
sDevHnd devHnd[] = {
  {NULL, pltCmd, NULL, 0, 0},
  {NULL, sfCmd, NULL, MB_DEV_SF, 1},
};

/**
 */
void platformSetup(void)
{
  int i;

  for (i=0; i<sizeof(devHnd)/sizeof(sDevHnd); i++) {
    if (devHnd[i].setup) {
      devHnd[i].setup(devHnd[i].sub_devs);
    }
  }
}

/**
 */
void platformCmd(void)
{
  volatile alt_u32 cmd;

  cmd = *(volatile alt_u32*)MB_BASE;
  if (cmd) {
    int dev;
    int i;
    dev = cmd & MB_DEV_MSK;
    for (i=0; i<sizeof(devHnd)/sizeof(sDevHnd); i++) {
      if (devHnd[i].dev_cod == dev) {
          if (devHnd[i].cmd) {
            devHnd[i].cmd();
            break;
          }
      }
    }
    *(volatile alt_u32*)MB_BASE = 0;
    intPinSet(1, 1);
    intPinSet(1, 0);
  }
}

/**
 */
void platformLoop(void)
{
  int i;

  for (i=0; i<sizeof(devHnd)/sizeof(sDevHnd); i++) {
    if (devHnd[i].loop) {
      devHnd[i].loop();
    }
  }
}

/**
 *
 */
void pltCmd(void)
{
  alt_u32 volatile *rpc = (alt_u32*)MB_BASE;
  alt_u32 ret;
  int i;

  ret = -1;
  switch(MB_CMD(rpc[0])){
  case 1:
    /* get version */
    ret = FPGA_VERSION;
    break;
  case 2:
    /* get IP list and number of instances for each device */
    rpc[1] = sizeof(devHnd)/sizeof(sDevHnd) -1;
    for (i=1; i<sizeof(devHnd)/sizeof(sDevHnd); i++) {
      rpc[i*2+0] = devHnd[i].dev_cod;
      rpc[i*2+1] = devHnd[i].sub_devs;
    }
    ret = rpc[1];
    break;
#if defined(RU_MODULE) && (RU_MODULE == 1)
  case 3:
    /* starting application */
    ret = ruLoad(0x00080000);
    break;
#endif /* defined(RU_MODULE) && (RU_MODULE == 1) */
  }
  rpc[1] = ret;
}
