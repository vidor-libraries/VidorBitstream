#include "sf.h"

alt_u32 sfSetup(alt_u32 cmd);

/**
 *
 */
int main(void)
{
  // enable quad spi access to flash
  sfSetup(0);
  sfEnableQuad();

  // jump to application in FLASH
  __asm__ volatile (
      "movhi r12, 0x008E\n"
      "ori r12, r12, 0x0000\n"
      "jmp r12");

  return 0;
}

