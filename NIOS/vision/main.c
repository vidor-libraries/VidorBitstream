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

  platformSetup();

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
