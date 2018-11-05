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

#ifndef ENC_H_
#define ENC_H_

#define ENC_UID 0x0D645

#define ENC_IP_VER   0x0105
#define ENC_DRV_VER  0x0004
#define ENC_VER (((ENC_IP_VER)<<16)|(ENC_DRV_VER))

#define ENC_CHNS   10

#define ENC_PIN_FNC_A     PIN_FNC(0x01, PIN_DIR_I)
#define ENC_PIN_FNC_B     PIN_FNC(0x02, PIN_DIR_I)

void encRpc(void);

#endif /* ENC_H_ */

