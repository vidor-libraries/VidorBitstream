/*
* Copyright 2018 ARDUINO SA (http://www.arduino.cc/)
* This file is part of Vidor IP.
* Copyright (c) 2018
* Authors: Dario Pennisi
*
* This software is released under:
* The GNU General Public License, which covers the main part of Vidor IP
* The terms of this license can be found at:
* https://www.gnu.org/licenses/gpl-3.0.en.html
*
* You can be released from the requirements of the above licenses by purchasing
* a commercial license. Buying such a license is mandatory if you want to modify or
* otherwise use the software for commercial activities involving the Arduino
* software without disclosing the source code of your own applications. To purchase
* a commercial license, send an email to license@arduino.cc.
*
*/

#ifndef I2C_H_
#define I2C_H_

#include <alt_types.h>

void i2cInit(int);
void i2cCmd(void);

alt_u32 i2cEnable(alt_u32 index);
alt_u32 i2cClockSet(alt_u32 index, alt_u32 baud);
alt_u32 i2cDisable(alt_u32 index);
alt_u32 i2cRead(alt_u32 index, alt_u8 address, alt_u8* data, alt_u32 len);
alt_u32 i2cWrite(alt_u32 index, alt_u8 address, alt_u8* data, alt_u32 len);

#endif /* I2C_H_ */
