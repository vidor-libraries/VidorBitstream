/**
 *
 */
#include <string.h>
#include <unistd.h>
#include <sys/alt_irq.h>

#include "sf.h"
#include "sign.h"

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
int main(void)
{
  int ret;

  // enable quad spi access to flash
  sfSetup(0);
  sfEnableQuad();

  // sign check
  ret = signChk();

  // delete executed code from RAM (text2 in boot)
  memset(SEC_START(text2), 0, SEC_SIZE(text2));
  memset(SEC_START(data2), 0, SEC_SIZE(data2));

  // check if sign is valid
  if(ret){
    while(1);
  }

  // disable all interrupt
//  alt_irq_disable_all();
  irqPinSet(0, NULL);

  // jump to application in FLASH
  __asm__ volatile (
      "movhi r12, 0x008E\n"
      "ori r12, r12, 0x0000\n"
      "jmp r12");

  return 0;
}

