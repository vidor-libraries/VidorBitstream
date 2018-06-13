/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2015 Altera Corporation, San Jose, California, USA.           *
* All rights reserved.                                                        *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
* The above copyright notice and this permission notice shall be included in  *
* all copies or substantial portions of the Software.                         *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
* This agreement shall be governed in all respects by the laws of the State   *
* of California and by the laws of the United States of America.              *
* Altera does not recommend, suggest or require that this reference design    *
* file be used in conjunction or combination with any other product.          *
******************************************************************************/

#ifndef ALTERA_REMOTE_UPDATE_REGS_H_
#define ALTERA_REMOTE_UPDATE_REGS_H_

#include "io.h"

/* */
#define ALTERA_RU_MASTER_SM_CURRENT_STATE_MODE_REG  0x00
#define IOADDR_ALTERA_RU_MASTER_SM_CURRENT_STATE_MODE(base)      \
        __IO_CALC_ADDRESS_NATIVE(base, ALTERA_RU_MASTER_SM_CURRENT_STATE_MODE_REG)
#define IORD_ALTERA_RU_MASTER_SM_CURRENT_STATE_MODE(base)  \
        IORD(base, ALTERA_RU_MASTER_SM_CURRENT_STATE_MODE_REG)

/* Trigger reconfig conditions */
#define ALTERA_RU_RECONFIG_TRIGGER_CONDITIONS_REG 0x18

#define IOADDR_ALTERA_RU_RECONFIG_TRIGGER_CONDITIONS(base)      \
        __IO_CALC_ADDRESS_NATIVE(base, ALTERA_RU_RECONFIG_TRIGGER_CONDITIONS_REG)
#define IORD_ALTERA_RU_RECONFIG_TRIGGER_CONDITIONS(base)  \
        IORD(base, ALTERA_RU_RECONFIG_TRIGGER_CONDITIONS_REG)

#define ALTERA_RU_RECONFIG_TRIGGER_CONDITIONS_MASK    0x1F
#define ALTERA_RU_RECONFIG_TRIGGER_CONDITIONS_RUNCONFIG  0x01
#define ALTERA_RU_RECONFIG_TRIGGER_CONDITIONS_WDTIMER   0x02
#define ALTERA_RU_RECONFIG_TRIGGER_CONDITIONS_NSTATUS   0x04
#define ALTERA_RU_RECONFIG_TRIGGER_CONDITIONS_CRCERROR  0x08
#define ALTERA_RU_RECONFIG_TRIGGER_CONDITIONS_NCONFIG   0x10

/* Watchdog timeout */
#define ALTERA_RU_WATCHDOG_TIMEOUT_REG  0x08

#define IOADDR_ALTERA_RU_WATCHDOG_TIMEOUT(base)      \
        __IO_CALC_ADDRESS_NATIVE(base, ALTERA_RU_WATCHDOG_TIMEOUT_REG)
#define IOWR_ALTERA_RU_WATCHDOG_TIMEOUT(base, data)  \
        IOWR(base, ALTERA_RU_WATCHDOG_TIMEOUT_REG, data)
#define IORD_ALTERA_RU_WATCHDOG_TIMEOUT(base)  \
        IORD(base, ALTERA_RU_WATCHDOG_TIMEOUT_REG)

#define ALTERA_RU_WATCHDOG_TIMEOUT_WRITE_MASK   0x00000FFF
#define ALTERA_RU_WATCHDOG_TIMEOUT_READ_MASK    0x1FFFFFFF


/* Watchdog enable */
#define ALTERA_RU_WATCHDOG_ENABLE_REG 0x0C

#define IOADDR_ALTERA_RU_WATCHDOG_ENABLE(base)      \
        __IO_CALC_ADDRESS_NATIVE(base, ALTERA_RU_WATCHDOG_ENABLE_REG)
#define IOWR_ALTERA_RU_WATCHDOG_ENABLE(base, data)  \
        IOWR(base, ALTERA_RU_WATCHDOG_ENABLE_REG, data)
#define IORD_ALTERA_RU_WATCHDOG_ENABLE(base)  \
        IORD(base, ALTERA_RU_WATCHDOG_ENABLE_REG)

#define ALTERA_RU_WATCHDOG_ENABLE_MASK    0x00000001
#define ALTERA_RU_WATCHDOG_ENABLE   0x1


/* Page select */
//RU_BOOT_ADDRESS
#define ALTERA_RU_PAGE_SELECT_REG 0x10
#define IOADDR_ALTERA_RU_PAGE_SELECT(base)      \
        __IO_CALC_ADDRESS_NATIVE(base, ALTERA_RU_PAGE_SELECT_REG)
#define IOWR_ALTERA_RU_PAGE_SELECT(base, data)  \
        IOWR(base, ALTERA_RU_PAGE_SELECT_REG, data)
#define IORD_ALTERA_RU_PAGE_SELECT(base)  \
        IORD(base, ALTERA_RU_PAGE_SELECT_REG)

#define ALTERA_RU_BOOT_ADDR_24_MASK   0x00FFFFFF

/* Reconfig mode */
#define ALTERA_RU_CONFIG_MODE_REG 0x04

#define IOADDR_ALTERA_RU_CONFIG_MODE(base)      \
        __IO_CALC_ADDRESS_NATIVE(base, ALTERA_RU_CONFIG_MODE_REG)
#define IOWR_ALTERA_RU_CONFIG_MODE(base, data)  \
        IOWR(base, ALTERA_RU_CONFIG_MODE_REG, data)
#define IORD_ALTERA_RU_CONFIG_MODE(base)  \
        IORD(base, ALTERA_RU_CONFIG_MODE_REG)

#define ALTERA_RU_RECONFIG_MODE_MASK    0x01
#define ALTERA_RU_RECONFIG_MODE_FACTORY   0x00
#define ALTERA_RU_RECONFIG_MODE_APP   0x01

/* Reset timer */
#define ALTERA_RU_RESET_TIMER_REG 0x1C
#define IOADDR_ALTERA_RU_RESET_TIMER(base)      \
        __IO_CALC_ADDRESS_NATIVE(base, ALTERA_RU_RESET_TIMER_REG)
#define IOWR_ALTERA_RU_RESET_TIMER(base, data)  \
        IOWR(base, ALTERA_RU_RESET_TIMER_REG, data)

/* Reconfig */
#define ALTERA_RU_RECONFIG_REG  0x1D

#define IOADDR_ALTERA_RU_RECONFIG(base)      \
        __IO_CALC_ADDRESS_NATIVE(base, ALTERA_RU_RECONFIG_REG)
#define IOWR_ALTERA_RU_RECONFIG(base, data)  \
        IOWR(base, ALTERA_RU_RECONFIG_REG, data)

/* Control Update */
#define ALTERA_CTL_NUPDT_REG 0x07 /*only applicable for A10 devices*/

#define IOADDR_ALTERA_CTL_NUPDT(base)      \
        __IO_CALC_ADDRESS_NATIVE(base, ALTERA_CTL_NUPDT_REG)
#define IOWR_ALTERA_CTL_NUPDT(base, data)  \
        IOWR(base, ALTERA_CTL_NUPDT_REG, data)

#define ALTERA_RU_CTL_NUPDT_MASK    0x01
#define ALTERA_RU_CONTROL_REG   0x01
#define ALTERA_RU_UPDATE_REG    0x00

#endif /*ALTERA_REMOTE_UPDATE_REGS_H_*/
