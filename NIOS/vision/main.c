/**
 *
 */
#include "platform.h"
#include "gpio.h"

/**
 *
 */
int main(void)
{
  platformSetup();

  irqPinSet(0, platformCmd);
  intPinInit(1, 0);

  while (1) {
    platformLoop();
  };
  return 0;
}

