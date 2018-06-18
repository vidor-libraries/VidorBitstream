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
#include "gpio.h"
#include "platform.h"

void cmdRx(alt_u32 cmd);

/**
 *
 */
int main()
{
  int ret;

  // calcola la firma
  ret = signChk();

  // cancella il codice eseguito fin qui (text2 in boot)
  memset((void*)BOOT_REGION_BASE, 0, BOOT_REGION_SPAN);

  // logo iniziale
  gfxInit(0);

  // verifica la validità della firma
  if(ret){
    fillRect(630, 0, 10, 10, 0xFC00);
    while(1);
  } else {
    fillRect(630, 0, 10, 10, 0x83E0);
  }

  irqPinSet(0, cmdRx);
  intPinInit(1, 0);

  platformSetup();
  while (1) {
    platformLoop();
  };

  return 0;
}

/**
 *
 */
void cmdRx(alt_u32 arg)
{
  platformCmd();
}
