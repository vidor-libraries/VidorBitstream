/*
 * pio.h
 *
 *  Created on: May 7, 2018
 *      Author: max
 */

#ifndef PIO_H_
#define PIO_H_

//#include <alt_types.h>


#define PIO_UID 0xE0C45

#define PIO_IP_VER   0x0105
#define PIO_DRV_VER  0x0004
#define PIO_VER     (((PIO_IP_VER)<<16)|(PIO_DRV_VER))

#define PIO_PIN_FNC_OUT   PIN_FNC(0x01, PIN_DIR_O)

void pioRpc(void);

alt_u32 pioMode(alt_u8 port, alt_u8 pin, alt_u8 mux, alt_u8 dir, alt_u8 val);

#endif /* PIO_H_ */
