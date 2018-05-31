/*
 * pio.h
 *
 *  Created on: May 31, 2018
 *      Author: max
 */

#ifndef PIO_H_
#define PIO_H_

/* PIO registers */
#define PIO_DATA  0x00		// data value
#define PIO_DIR   0x01		// direction
#define PIO_IMSK  0x02		// interrupt mask
#define PIO_EDET  0x03		// edge detection
#define PIO_SET   0x04		// bit of output port to set
#define PIO_CLR   0x05		// bit of output port to clear

#define SPI_MODE_PIN 2

#endif /* PIO_H_ */
