#ifndef _VIDOR_ENC_H
#define _VIDOR_ENC_H

#include "Arduino.h"
#include "VidorUtils.h"
#include "defines.h"

#define MB_DEV_ENC  0x0A000000

class VidorEncoder {
  public:
    VidorEncoder(int index);
    void write(int32_t p);
    int32_t read();
  protected:
    int32_t offset=0;
    int idx;
};


#endif //_VIDOR_ENC_H
