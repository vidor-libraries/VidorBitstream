/*
 * mb.h
 *
 *  Created on: Apr 11, 2018
 *      Author: max
 */

#ifndef MB_H_
#define MB_H_

#include <alt_types.h>
#include <system.h>

/**
 *
 */
#define MB_BASE (0x80000000 | DPRAM_BASE)

/**
 * Device, Subdev, Command
 */
#define MB_DEV_MSK  0xFF000000
#define MB_DEV_OFS  24
#define MB_SUB_MSK  0x00F00000
#define MB_SUB_OFS  20
#define MB_CMD_MSK  0x000FFFFF
#define MB_CMD_OFS  0

#define MB_DEV(a) ((a & MB_DEV_MSK) >> MB_DEV_OFS)
#define MB_SUB(a) ((a & MB_SUB_MSK) >> MB_SUB_OFS)
#define MB_CMD(a) ((a & MB_CMD_MSK) >> MB_CMD_OFS)

#define MB_DEV_SF     (0x01<<MB_DEV_OFS)
#define MB_DEV_GPIO   (0x02<<MB_DEV_OFS)
#define MB_DEV_GFX    (0x03<<MB_DEV_OFS)
#define MB_DEV_I2C    (0x04<<MB_DEV_OFS)
#define MB_DEV_SPI    (0x05<<MB_DEV_OFS)
#define MB_DEV_UART   (0x06<<MB_DEV_OFS)
#define MB_DEV_QR     (0x07<<MB_DEV_OFS)
#define MB_DEV_SDRAM  (0x08<<MB_DEV_OFS)
#define MB_DEV_NP     (0x09<<MB_DEV_OFS)
#define MB_DEV_ENC    (0x0A<<MB_DEV_OFS)
#define MB_DEV_REG    (0x0B<<MB_DEV_OFS)

#endif /* MB_H_ */
