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

#define SEC_START(section) (&_alt_partition_##section##_start)
#define SEC_SIZE(section) (&_alt_partition_##section##_end-&_alt_partition_##section##_start)

#define SEC_EXTERN(section) \
    extern void _alt_partition_##section##_start;                 \
    extern void _alt_partition_##section##_end;                   

SEC_EXTERN(text2)
SEC_EXTERN(data2)

/**
 *
 */
int main()
{
  int ret;

  // calcola la firma
  ret = signChk();

  // cancella il codice eseguito fin qui (text2 in boot)
  memset(SEC_START(text2), 0, SEC_SIZE(text2));
  memset(SEC_START(data2), 0, SEC_SIZE(data2));

  // logo iniziale
  gfxInit(0);

  // verifica la validità della firma
  if(ret){
    fillRect(630, 0, 10, 10, 0xFC00);
    while(1);
  } else {
    fillRect(630, 0, 10, 10, 0x83E0);
  }
  // start command manager
  irqPinSet(0, platformCmd);
  intPinInit(1, 0);

  platformSetup();
  while (1) {
    platformLoop();
  };

  return 0;
}
