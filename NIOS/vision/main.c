/**
*
*/
#include <string.h>

#include "platform.h"
#include "gpio.h"


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
