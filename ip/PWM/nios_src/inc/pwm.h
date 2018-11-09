/**
 * Copyright 2018 ARDUINO SA (http://www.arduino.cc/)
 * This file is part of Vidor IP.
 * Copyright (c) 2018
 * Authors: Massimiliano Agneni
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
 * @file pwm.h
 * @brief
 */

#ifndef PWM_H_
#define PWM_H_

#define PWM_UID 0x08A55

#define PWM_IP_VER   0x0105
#define PWM_DRV_VER  0x0004
#define PWM_VER (((PWM_IP_VER)<<16)|(PWM_DRV_VER))

#define PWM_PIN_FNC_OUT   PIN_FNC(0x01, PIN_DIR_O)

void pwmRpc(void);

#endif /* PWM_H_ */
