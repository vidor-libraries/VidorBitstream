#ifndef _VIDOR_NEOPIX_H
#define _VIDOR_NEOPIX_H

#include "Arduino.h"
#include "VidorUtils.h"

#define MB_DEV_NP  0x09000000

class VidorNeopixel {
  public:
    VidorNeopixel(void);
    uint32_t npSet(uint32_t msk, uint16_t LEDs, uint8_t type);
    uint32_t npLedSet(uint32_t idx, uint16_t LED, uint32_t red, uint32_t green, uint32_t blue, uint32_t white);
    uint32_t npBrgSet(uint32_t idx, uint16_t brg);
    uint32_t npLedShow(void);
    uint32_t npTest(void);
  protected:
};



#endif //_VIDOR_NEOPIX_H
