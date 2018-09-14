/*
 * pwm.h
 *
 *  Created on: Aug 29, 2018
 *      Author: max
 */

#ifndef PWM_H_
#define PWM_H_

void pwmCmd(void);

alt_u32 pwmFrqSet(alt_u32 prescaler, alt_u16 period);
alt_u32 pwmWrite(alt_u32 pin, alt_u16 mh, alt_u16 ml);

#endif /* PWM_H_ */
