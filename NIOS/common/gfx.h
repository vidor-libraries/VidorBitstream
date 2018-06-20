/*
 * gfx.h
 *
 *  Created on: Apr 11, 2018
 *      Author: max
 */

#ifndef GFX_H_
#define GFX_H_

#include <alt_types.h>

void gfxInit(int devs);
void gfxCmd(void);

alt_u32 fillRect(alt_u16 x, alt_u16 y, alt_u16 w, alt_u16 h, alt_u16 color);

#endif /* GFX_H_ */
