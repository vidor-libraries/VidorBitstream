/*
  This file is part of the VidorPeripherals/VidorGraphics library.
  Copyright (c) 2018 Arduino SA. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _VIDOR_ENC_H
#define _VIDOR_ENC_H

#include "Arduino.h"
#include "defines.h"

class VidorEncoder {
  public:
    VidorEncoder(int index);
    void write(int32_t p);
    int32_t read();
  protected:
    int32_t offset = 0;
    int idx;
    uint8_t devIdx;
};


#endif //_VIDOR_ENC_H
