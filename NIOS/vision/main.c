/**
*
*/
#include "platform.h"
#include "gpio.h"


//#define NP_TEST

#ifdef NP_TEST

alt_u32 npSet(alt_u32 mask, alt_u16 numLEDs, alt_u8 fmt, alt_u8 speed);
alt_u32 npLedSet(alt_u32 index, alt_u16 LED, alt_u32 color, alt_u32 flags);
alt_u32 npLedShow(void);

void npTest(void)
{
  npSet(0x0000000F, 5, 1, 2);
  npLedSet(0, 0, 0x007F0000, 0);
  npLedSet(0, 1, 0x00007F00, 0);
  npLedSet(0, 2, 0x0000007F, 0);
  npLedSet(0, 3, 0x007F007F, 0);
  npLedSet(0, 4, 0x007F7F00, 0);

  npLedSet(1, 0, 0x007F007F, 0);
  npLedSet(1, 1, 0x0000007F, 0);
  npLedSet(1, 2, 0x00007F00, 0);
  npLedSet(1, 3, 0x007F0000, 0);
  npLedSet(1, 4, 0x007F7F00, 0);

  npLedSet(1, 0, 0x007F007F, 0);
  npLedSet(1, 1, 0x0000007F, 0);
  npLedSet(1, 2, 0x00007F00, 0);
  npLedSet(1, 3, 0x007F0000, 0);
  npLedSet(1, 4, 0x007F7F00, 0);

  npLedSet(1, 0, 0x007F007F, 0);
  npLedSet(1, 1, 0x0000007F, 0);
  npLedSet(1, 2, 0x00007F00, 0);
  npLedSet(1, 3, 0x007F0000, 0);
  npLedSet(1, 4, 0x007F7F00, 0);

  for(;;){
    npLedShow();
  }
}

#endif
/**
 *
 */
int main(void)
{
#ifdef NP_TEST
  npTest();
#endif
  platformSetup();

  irqPinSet(0, platformCmd);
  intPinInit(1, 0);

  while (1) {
    platformLoop();
  };
  return 0;
}

