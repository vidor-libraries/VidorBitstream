/*
 * irq.h
 *
 *  Created on: Aug 29, 2018
 *      Author: max
 */
#ifndef IRQ_H_
#define IRQ_H_

#include <alt_types.h>

alt_u32 irqPinSet(alt_u32 pin, void (*hook)(void));
alt_u32 irqPinDet(alt_u32 pin);
alt_u32 irqPinClr(alt_u32 pin);

int intPinInit(alt_u32 pin, int val);
int intPinSet(alt_u32 pin, int val);

#endif /* IRQ_H_ */
