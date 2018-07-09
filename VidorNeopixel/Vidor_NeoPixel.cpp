#include "Vidor_NeoPixel.h"
#include "VidorIO.h"

Vidor_NeoPixel::Vidor_NeoPixel(uint8_t pin, uint16_t howMany, uint8_t type = NEO_GRB + NEO_KHZ800) {
  uint32_t ptr[4];

  switch (pin) {
    case NEOPIXEL_PIN_0:
      index = 0;
      break;
    case NEOPIXEL_PIN_1:
      index = 1;
      break;
    case NEOPIXEL_PIN_2:
      index = 2;
      break;
    case NEOPIXEL_PIN_3:
      index = 3;
      break;
    case NEOPIXEL_PIN_4:
      index = 4;
      break;
    case NEOPIXEL_PIN_5:
      index = 5;
      break;
    default:
      // no default case allowed
      return;
  }

  if (pin >= A0) {
    msk = 1 << (pin - A0 + 1);
    pinMode(pin - A0 + 132 + 1, 5)
  } else {
    msk = 1 << (pin + 8);
    pinMode(pin + 132 + 8, 5)
  }

  this->howMany = howMany;
  this->type = type;
}

uint32_t Vidor_NeoPixel::begin()
{
  uint32_t ptr[4];

  ptr[0] = MB_DEV_NP | 1;
  ptr[1] = msk & 0x007FFFFF;
  ptr[2] = howMany;
  ptr[3] = type;

  init = true;

  return mbCmdSend(ptr, 4);
}

/**
 *
 */
uint32_t Vidor_NeoPixel::setPixelColor(uint16_t n, uint32_t red, uint32_t green, uint32_t blue, uint32_t white)
{

  if (!init) {
    begin();
  }

  uint32_t ptr[6];

  ptr[0] = MB_DEV_NP | ((index & 0x0F)<<20) | 2;
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
uint32_t Vidor_NeoPixel::setBrightness(uint16_t brg)
{

  if (!init) {
    begin();
  }

  uint32_t ptr[2];

  ptr[0] = MB_DEV_NP | ((index & 0x0F)<<20) | 3;
  ptr[1] = brg;

  return mbCmdSend(ptr, 2);
}

/**
 *
 */
uint32_t Vidor_NeoPixel::show(void)
{

  if (!init) {
    begin();
  }

  uint32_t ptr[1];

  ptr[0] = MB_DEV_NP | 4;

  return mbCmdSend(ptr, 1);
}

/**
 */
uint32_t Vidor_NeoPixel::test(void)
{
  // call test() on a device created with Vidor_NeoPixel(A0, 256, NEO_BRG)

  //setPin(0x00000001, 256, 0x0058);

  for (int i=0; i<256; i++) {
    setPixelColor(0, i, i*4, 255-i*4, i*8, 0);
  }
  show();
  return 0;

}
