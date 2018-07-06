#include "VidorNeopixel.h"

VidorNeopixel::VidorNeopixel(void){

}

uint32_t VidorNeopixel::npSet(uint32_t msk, uint16_t LEDs, uint8_t type)
{
  uint32_t ptr[4];

  ptr[0] = MB_DEV_NP | 1;
  ptr[1] = msk & 0x007FFFFF;
  ptr[2] = LEDs;
  ptr[3] = type;

  return mbCmdSend(ptr, 4);
}

/**
 *
 */
uint32_t VidorNeopixel::npLedSet(uint32_t idx, uint16_t LED, uint32_t red, uint32_t green, uint32_t blue, uint32_t white)
{
  uint32_t ptr[6];

  ptr[0] = MB_DEV_NP | ((idx & 0x0F)<<20) | 2;
  ptr[1] = LED;
  ptr[2] = red;
  ptr[3] = green;
  ptr[4] = blue;
  ptr[5] = white;

  return mbCmdSend(ptr, 6);
}

/**
 *
 */
uint32_t VidorNeopixel::npBrgSet(uint32_t idx, uint16_t brg)
{
  uint32_t ptr[2];

  ptr[0] = MB_DEV_NP | ((idx & 0x0F)<<20) | 3;
  ptr[1] = brg;

  return mbCmdSend(ptr, 2);
}

/**
 *
 */
uint32_t VidorNeopixel::npLedShow(void)
{
  uint32_t ptr[1];

  ptr[0] = MB_DEV_NP | 4;

  return mbCmdSend(ptr, 1);
}

/**
 */
uint32_t VidorNeopixel::npTest(void)
{
  int i;
  npSet(0x00000001, 256, 0x0058);

  for (i=0; i<256; i++) {
    npLedSet(0, i, i*4, 255-i*4, i*8, 0);
  }
  npLedShow();
  return 0;

}
