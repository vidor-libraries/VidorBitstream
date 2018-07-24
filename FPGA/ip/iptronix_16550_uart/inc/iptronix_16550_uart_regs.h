/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2013 Altera Corporation, San Jose, California, USA.           *
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
*                                                                             *
******************************************************************************/
#ifndef	__ALT_16550_UART_REGS_H__
#define	__ALT_16550_UART_REGS_H__

#include <io.h>

#define IOADDR_ALTERA_16550_UART(base)          \
        __IO_CALC_ADDRESS_NATIVE(base, 0)

#define ALTERA_16550_UART_RBR_OFST              0x00
#define IORD_ALTERA_16550_UART_RBR(base)        \
        IORD(base, ALTERA_16550_UART_RBR_OFST)

#define ALTERA_16550_UART_THR_OFST              0x00
#define IOWR_ALTERA_16550_UART_THR(base, data)  \
        IOWR(base, ALTERA_16550_UART_THR_OFST, data)
#define ALTERA_16550_UART_DLL_OFST              0x00
#define IOWR_ALTERA_16550_UART_DLL(base, data)  \
        IOWR(base, ALTERA_16550_UART_DLL_OFST, data)

#define ALTERA_16550_UART_IER_OFST              0x01
#define IOWR_ALTERA_16550_UART_IER(base, data)  \
        IOWR(base, ALTERA_16550_UART_IER_OFST, data)
#define IORD_ALTERA_16550_UART_IER(base)  \
        IORD(base, ALTERA_16550_UART_IER_OFST)
#define ALTERA_16550_UART_IER_ELSI_MSK          0x00000004    /* Receiver Line Status */
#define ALTERA_16550_UART_IER_ETBEI_MSK         0x00000002    /* Transmit Holding Register Empty */
#define ALTERA_16550_UART_IER_ERBFI_MSK         0x00000001    /* Received Data Available */

#define ALTERA_16550_UART_DLH_OFST              0x01
#define IOWR_ALTERA_16550_UART_DLH(base, data)  \
        IOWR(base, ALTERA_16550_UART_DLH_OFST, data)

#define ALTERA_16550_UART_IIR_OFST              0x02
#define IORD_ALTERA_16550_UART_IIR(base)  \
        IORD(base, ALTERA_16550_UART_IIR_OFST)
#define ALTERA_16550_UART_FCR_OFST              0x02
#define IOWR_ALTERA_16550_UART_FCR(base, data)  \
        IOWR(base, ALTERA_16550_UART_FCR_OFST, data)
#define ALTERA_16550_UART_FCR_XFIFOR_MSK       0x00000004    /* Tx FIFO reset */
#define ALTERA_16550_UART_FCR_FIFOR_MSK        0x00000002    /* Rx FIFO reset */
#define ALTERA_16550_UART_FCR_RT_MSK           0x000000C0    /* Rx Trigger Level */
#define ALTERA_16550_UART_FCR_TET_MSK          0x00000030    /* Tx Trigger Level */
#define ALTERA_16550_UART_FCR_FIFOE_MSK        0x00000001    /* FIFO enable */

#define ALTERA_16550_UART_LCR_OFST             0x03
#define IORD_ALTERA_16550_UART_LCR(base)       \
        IORD(base, ALTERA_16550_UART_LCR_OFST)
#define IOWR_ALTERA_16550_UART_LCR(base, data)  \
        IOWR(base, ALTERA_16550_UART_LCR_OFST, data)
#define ALTERA_16550_UART_LCR_DLAB_MSK         0x00000080    /* Divisor Latch Access Bit */
#define ALTERA_16550_UART_LCR_BC_MSK           0x00000040    /* Break Control Bit */
#define ALTERA_16550_UART_LCR_EPS_MSK          0x00000010    /* Even Parity Select */
#define ALTERA_16550_UART_LCR_PEN_MSK          0x00000008    /* Parity Enable */
#define ALTERA_16550_UART_LCR_STOP_MSK         0x00000004    /* Stop Bit */
#define ALTERA_16550_UART_LCR_DLS_MSK          0x00000103    /* Data Length Select */

#define ALTERA_16550_UART_MCR_OFST             0x4
#define IOWR_ALTERA_16550_UART_MCR(base, data)  \
        IOWR(base, ALTERA_16550_UART_MCR_OFST, data)
#define IORD_ALTERA_16550_UART_MCR(base)        \
        IORD(base, ALTERA_16550_UART_MCR_OFST)
#define ALTERA_16550_UART_MCR_AFCE_MSK          0x00000020     /* Hardware Auto Flow Control */
#define ALTERA_16550_UART_MCR_RTS_MSK           0x00000002     /* Request to send */
#define ALTERA_16550_UART_MCR_DTR_MSK           0x00000001     /* Data Terminal Ready */

#define ALTERA_16550_UART_LSR_OFST              0x5
#define IORD_ALTERA_16550_UART_LSR(base)        \
        IORD(base, ALTERA_16550_UART_LSR_OFST)
#define IOWR_ALTERA_16550_UART_LSR(base, data)  \
        IOWR(base, ALTERA_16550_UART_LSR_OFST, data)
#define ALTERA_16550_UART_LSR_RFE_MSK           0x00000001    /* Receiver FIFO Error */
#define ALTERA_16550_UART_LSR_TEMT_MSK          0x00000040    /* Transmitter Empty */
#define ALTERA_16550_UART_LSR_THRE_MSK          0x00000020    /* Transmit Holding Register Empty */
#define ALTERA_16550_UART_LSR_BI_MSK            0x00000010    /* Break Interrupt */
#define ALTERA_16550_UART_LSR_FE_MSK            0x00000008    /* Framing Error */
#define ALTERA_16550_UART_LSR_PE_MSK            0x00000004    /* Parity Error */
#define ALTERA_16550_UART_LSR_OE_MSK            0x00000002    /* Overrun Error */
#define ALTERA_16550_UART_LSR_DR_MSK            0x00000001    /* Data Ready */
#define ALTERA_16550_UART_LCR_STICK_MSK         0X00000020    /* Stick Parity Bit */

#define ALTERA_16550_UART_MSR_OFST              0x6
#define IORD_ALTERA_16550_UART_MSR(base)        \
        IORD(base, ALTERA_16550_UART_MSR_OFST)
#define ALTERA_16550_UART_MSR_CTS_MSK           0x00000001     /* Clear to Send */

#define ALTERA_16550_UART_SCR_OFST              0x7

#endif /* __ALTERA_16550_UART_REGS_H__ */
