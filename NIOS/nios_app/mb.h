/*
 * mb.h
 *
 *  Created on: Apr 11, 2018
 *      Author: max
 */

#ifndef MB_H_
#define MB_H_

#include <alt_types.h>

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

#define MB_DEV_SF     (1<<MB_DEV_OFS)
#define MB_DEV_GPIO   (2<<MB_DEV_OFS)
#define MB_DEV_GFX    (3<<MB_DEV_OFS)
#define MB_DEV_I2C    (4<<MB_DEV_OFS)
#define MB_DEV_SPI    (5<<MB_DEV_OFS)
#define MB_DEV_UART   (6<<MB_DEV_OFS)
#define MB_DEV_QR     (7<<MB_DEV_OFS)
#define MB_DEV_SDRAM  (8<<MB_DEV_OFS)

#endif /* MB_H_ */
