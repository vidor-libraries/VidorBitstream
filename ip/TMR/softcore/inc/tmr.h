/*
 * tmr.h
 *
 *  Created on: Jun 12, 2018
 *      Author: max
 */

#ifndef TMR_H_
#define TMR_H_

#include <alt_types.h>

alt_u32 tmrStart(alt_u32 ms, void (*hook)(void*), void* arg);


#endif /* TMR_H_ */
