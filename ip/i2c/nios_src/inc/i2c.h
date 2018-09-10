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

#define I2C_BASE  0xa0554
#define I2C_UID   0x15540

#define I2C_IP_VER   0x0101
#define I2C_DRV_VER  0x0203
#define I2C_VER      (((I2C_IP_VER)<<16)|(I2C_DRV_VER))

#define I2C_0_CHNS  1
#define I2C_1_CHNS  1

#define I2C_PIN_FNC_SCL PIN_FNC(0x01, PIN_DIR_O)
#define I2C_PIN_FNC_SDA PIN_FNC(0x02, PIN_DIR_I)


void i2cRpc(void);

#endif /* I2C_H_ */
