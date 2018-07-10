#include "Vidor_NeoPixel.h"
#include "VidorIO.h"

Vidor_NeoPixel::Vidor_NeoPixel(uint16_t howMany, uint8_t pin, uint8_t type) {

  this->howMany = howMany;
  this->type = type;
  this->pin = pin;
}

uint32_t Vidor_NeoPixel::begin()
{
  uint32_t ptr[4];

  switch (pin) {
#ifdef NEOPIXEL_PIN_0
    case NEOPIXEL_PIN_0:
      index = 0;
      break;
#endif
#ifdef NEOPIXEL_PIN_1
    case NEOPIXEL_PIN_1:
      index = 1;
      break;
#endif
#ifdef NEOPIXEL_PIN_2
    case NEOPIXEL_PIN_2:
      index = 2;
      break;
#endif
#ifdef NEOPIXEL_PIN_3
    case NEOPIXEL_PIN_3:
      index = 3;
      break;
#endif
#ifdef NEOPIXEL_PIN_4
    case NEOPIXEL_PIN_4:
      index = 4;
      break;
#endif
#ifdef NEOPIXEL_PIN_5
    case NEOPIXEL_PIN_5:
      index = 5;
      break;
#endif
#ifdef NEOPIXEL_PIN_6
    case NEOPIXEL_PIN_6:
      index = 6;
      break;
#endif
#ifdef NEOPIXEL_PIN_7
    case NEOPIXEL_PIN_7:
      index = 7;
      break;
#endif
#ifdef NEOPIXEL_PIN_8
    case NEOPIXEL_PIN_8:
      index = 8;
      break;
#endif
#ifdef NEOPIXEL_PIN_9
    case NEOPIXEL_PIN_9:
      index = 9;
      break;
#endif
#ifdef NEOPIXEL_PIN_10
    case NEOPIXEL_PIN_10:
      index = 10;
      break;
#endif
    default:
      // no default case allowed
      return -1;
  }

  if (pin >= A0) {
    msk = 1 << (pin - A0);
    pinMode(pin - A0 + 132 + 1, NEOPIXEL_PINMUX);
  } else {
    msk = 1 << (pin + 7);
    pinMode(pin + 132 + 8, NEOPIXEL_PINMUX);
  }

  ptr[0] = MB_DEV_NP | 1;
  ptr[1] = msk & 0x007FFFFF;
  ptr[2] = howMany;
  ptr[3] = type;

  init = true;

  return mbCmdSend(ptr, 4);
}

uint32_t Vidor_NeoPixel::setPin(uint8_t pin) {
  this->pin = pin;
  begin();
  return 0;
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
  ptr[1] = n;
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
  // call test() on a device created with Vidor_NeoPixel(128, A0, NEO_BRG)

  //setPin(0x00000001, 256, 0x0058);

  for (int i=0; i<256; i++) {
    setPixelColor(i, i*4, 255-i*4, i*8, 0);
  }
  show();
  return 0;

}
