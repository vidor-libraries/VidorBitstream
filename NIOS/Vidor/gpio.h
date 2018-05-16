/*
 * gpio.h
 *
 *  Created on: May 7, 2018
 *      Author: max
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <alt_types.h>

void gpioCmd(void);

alt_u32 irqPinSet(alt_u32 pin, void (*hook)(alt_u32));

#endif /* GPIO_H_ */
