/**
*
*/
#include <string.h>

#include "sign.h"

#define SEC_START(section) (&_alt_partition_##section##_start)
#define SEC_SIZE(section) (&_alt_partition_##section##_end-&_alt_partition_##section##_start)

#define SEC_EXTERN(section) \
    extern void _alt_partition_##section##_start;                 \
    extern void _alt_partition_##section##_end;

SEC_EXTERN(text2)
SEC_EXTERN(data2)

#define TEST

#ifdef TEST

#include "platform.h"
#include "gpio.h"

void cmdRx(alt_u32 cmd);

#endif /* TEST */

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

  // verifica la validità della firma
  if(ret){
    while(1);
  }

#ifdef TEST
  platformSetup();
  irqPinSet(0, cmdRx);
  intPinInit(1, 0);
  while (1) {
    platformLoop();
  };
#else
  /* disable interrupt */
  alt_irq_disable_all();

  /* start application */
  __asm__ volatile (
      "movhi r12, 0x100E\n"
      "ori r12, r12, 0x0000\n"
      "jmp r12");
#endif /* TEST */
  return 0;
}

#ifdef TEST
/**
 *
 */
void cmdRx(alt_u32 arg)
{
  platformCmd();
}
#endif /* TEST */
