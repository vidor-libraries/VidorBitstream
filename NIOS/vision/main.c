/**
*
*/
#include <string.h>

#include "platform.h"
#include "gfx.h"
#include "gpio.h"

#define SEC_START(section) (&_alt_partition_##section##_start)
#define SEC_SIZE(section) (&_alt_partition_##section##_end-&_alt_partition_##section##_start)

#define SEC_EXTERN(section) \
    extern void _alt_partition_##section##_start;                 \
    extern void _alt_partition_##section##_end;                   

SEC_EXTERN(text2)
SEC_EXTERN(data2)

void cmdRx(alt_u32 cmd);

/**
 *
 */
int main(void)
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

  platformSetup();
  //aesTest();
  //i2cTest();
  //gpioTest();
  //sfTest();
  //i2c1Test();
  //uartTest();
  //aesTest();

  irqPinSet(0, cmdRx);
  intPinInit(1, 0);

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
