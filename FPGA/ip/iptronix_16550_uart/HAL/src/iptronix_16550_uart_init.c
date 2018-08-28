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

#include <fcntl.h>

#include "sys/alt_irq.h"
#include "sys/ioctl.h"
#include "sys/alt_errno.h"
#include "iptronix_16550_uart.h"
#include "iptronix_16550_uart_regs.h"
#include  <sys/alt_llist.h>

ALT_LLIST_HEAD(alt_16550_uart_list);

#define SEC_RAM  __attribute__((__section__(".rwdata")))

/*
 * altera_16550_uart_rxirq() is called by altera_16550_uart_irq() to
 * process a receive interrupt. It transfers the incoming character into
 * the receive circular buffer, and sets the appropriate flags to indicate
 * that there is data ready to be processed.
 */
void SEC_RAM altera_16550_uart_rxirq(altera_16550_uart_state* sp)
{
    alt_u32 next;
    alt_u32 regs;

    if(!(sp->error_ignore))
    {
    	/* If there was an error, discard the data */
    	if (sp->line_status.pe |
		sp->line_status.fe |
        	sp->line_status.bi)
    	{
		/* discard data from RBR/ 1st byte data at the top of FIFO */
    		IORD_ALTERA_16550_UART_RBR(sp->base);
    		return;
    	}
    }

    /*
     * In a multi-threaded environment, set the read event flag to indicate
     * that there is data ready. This is only done if the circular buffer was
     * previously empty.
     */

    if (sp->rx_end == sp->rx_start)
    {
        ALT_FLAG_POST (sp->events, ALT_16550_UART_READ_RDY, OS_FLAG_SET);
    }

    /* Determine which slot to use next in the circular buffer */
    next = (sp->rx_end + 1) & ALT_16550_UART_BUF_MSK;

    if (sp->config.fifo_mode)
    {
        /* Continuously read from RBR till DR is clear to 0*/
        do
        {
            /* Transfer data from the device to the circular buffer */
            sp->rx_buf[sp->rx_end] = IORD_ALTERA_16550_UART_RBR(sp->base);
            sp->rx_end = next;

            next = (sp->rx_end + 1) & ALT_16550_UART_BUF_MSK;
            regs = IORD_ALTERA_16550_UART_LSR (sp->base);

        } while ((regs & ALTERA_16550_UART_LSR_DR_MSK) && (next != sp->rx_start));

    } else
    {
        /* Transfer data from the device to the circular buffer */
        sp->rx_buf[sp->rx_end] = IORD_ALTERA_16550_UART_RBR(sp->base);

        sp->rx_end = next;

        next = (sp->rx_end + 1) & ALT_16550_UART_BUF_MSK;
    }
/* MAX */
#if 0
    /*
     * If the cicular buffer was full, disable interrupts. Interrupts will be
     * re-enabled when data is removed from the buffer.
     */
    if (next == sp->rx_start)
    {
        sp->ctrl &= ~ALTERA_16550_UART_IER_ERBFI_MSK;
        IOWR_ALTERA_16550_UART_IER(sp->base, sp->ctrl);
    }
#endif
/* MAX */
}


/*
 * altera_16550_uart_txirq() is called by altera_16550_uart_irq() to
 * process a transmit interrupt. It transfers data from the transmit
 * buffer to the device, and sets the apropriate flags to indicate that
 * there is data ready to be processed.
 */
void
altera_16550_uart_txirq(altera_16550_uart_state* sp)
{
    alt_u32 regs;
    /* Transfer data if there is some ready to be transfered */

    if (sp->tx_start != sp->tx_end)
    {
        /*
         * In a multi-threaded environment, set the write event flag to indicate
         * that there is space in the circular buffer. This is only done if the
         * buffer was previously empty.
         */
        if (sp->tx_start == ((sp->tx_end + 1) & ALT_16550_UART_BUF_MSK))
        {
            ALT_FLAG_POST (sp->events,
                           ALT_16550_UART_WRITE_RDY,
                           OS_FLAG_SET);
        }
        /* Write the data to the device */
        /* If fifo enable continuously drain circular buffer to tx fifo */
        regs = IORD_ALTERA_16550_UART_LSR(sp->base);
        if (sp->config.fifo_mode)
        {
            /* Fill up Tx FIFO from circular buffer until TX FIFO FULl
             * or circular buffer is empty
             */
            do
            {
            	if(regs & ALTERA_16550_UART_LSR_THRE_MSK)
            	{
            		IOWR_ALTERA_16550_UART_THR(sp->base, sp->tx_buf[sp->tx_start]);
            		sp->tx_start = (++sp->tx_start) & ALT_16550_UART_BUF_MSK;
            	}
            	regs = IORD_ALTERA_16550_UART_LSR(sp->base);
            } while (sp->tx_start != sp->tx_end);
        } else
        {
/* MAX */
            //IOWR_ALTERA_16550_UART_THR(sp->base, sp->tx_buf[sp->tx_start]);
alt_u32 data;
data = (alt_u8)sp->tx_buf[sp->tx_start];
IOWR_ALTERA_16550_UART_THR(sp->base, data);
/* MAX */
            sp->tx_start = (++sp->tx_start) & ALT_16550_UART_BUF_MSK;
            /*
             * In case the transmit interrupt had previously been disabled by
             * detecting a low value on CTS, it is re-enabled here.
             */
            sp->ctrl |= ALTERA_16550_UART_IER_ETBEI_MSK;
        }

    }


    /*
     * If the circular buffer is empty, disable the interrupt. This will be
     * re-enabled when new data is placed in the buffer.
     */

    if (sp->tx_start == sp->tx_end)
    {
        sp->ctrl &= ~(ALTERA_16550_UART_IER_ETBEI_MSK);
    }
    IOWR_ALTERA_16550_UART_IER(sp->base, sp->ctrl);
}

/*
 * altera_16550_uart_irq() is the interrupt handler registered at
 * configuration time for processing UART interrupts. It vectors
 * interrupt requests to either altera_16550_uart_rxirq() (for incoming
 * data), or altera_16550_uart_txirq() (for outgoing data).
 */
#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
static void SEC_RAM altera_16550_uart_irq(void* context)
#else
static void SEC_RAM altera_16550_uart_irq(void* context, alt_u32 id)
#endif
{
    alt_u32 status = 0;
    altera_16550_uart_state* sp = (altera_16550_uart_state*) context;
    void* base               = sp->base;

    /*
     * Read the status register in order to determine the cause of the
     * interrupt.
     */
    status = IORD_ALTERA_16550_UART_LSR(base);
    sp->line_status.dr = status;
    sp->line_status.oe = status >> 1;
    sp->line_status.pe = status >> 2;
    sp->line_status.fe = status >> 3;
    sp->line_status.bi = status >> 4;
    sp->line_status.thre = status >> 5;
    sp->line_status.temt = status >> 6;
    sp->line_status.rfe = status >> 7;

    /* Dummy read to ensure IRQ is negated before ISR returns */
    IORD_ALTERA_16550_UART_LSR(base);

    /* process a read irq */
    if (sp->line_status.dr)
    {
        altera_16550_uart_rxirq(sp);
    }

    /* process a write irq */
    if (sp->line_status.thre)
    {
        altera_16550_uart_txirq(sp);
    }
}

/*
 * The close() routine is implemented to drain the UART transmit buffer
 * This routine will wait for transimt data to beemptied unless the driver
 * flags have been set to non-blocking mode.This routine should be called
 * indirectly (i.e. though the C library close() routine) so that the file
 * descriptor associated with the relevant stream (i.e. stdout) can be closed
 * as well. This routine does not manage file descriptors.
 */
int altera_16550_uart_close(altera_16550_uart_state* sp, int flags)
{
    /*
     * Wait for all transmit data to be emptied by the UART ISR.
     */
    while (sp->tx_start != sp->tx_end)
    {
        if (flags & O_NONBLOCK)
        {
        return -1;
        }
    }
  return 0;
}

altera_16550_uart_state* altera_16550_uart_open (const char *name)
{
    altera_16550_uart_state *sp;

    /* Find requested device */
    sp = (altera_16550_uart_state*) alt_find_dev (name, &alt_16550_uart_list);
    if (sp == NULL)
    {
        return NULL;
    }
    return sp;
}

alt_u32 alt_16550_uart_config(altera_16550_uart_state* sp, UartConfig *setting)
{
    alt_u32 regs;
    alt_u32 divisor;
    alt_u32 LCR = IORD_ALTERA_16550_UART_LCR(sp->base);

    if (sp->config.baudrate != setting->baudrate)
    {
        /* Set LCR [7] to program Divisor */
        IOWR_ALTERA_16550_UART_LCR(sp->base, (LCR | ALTERA_16550_UART_LCR_DLAB_MSK));
        // Formula for calculating the divisor:
        //    baudrate = clock / (16 * divisor)
        // => baudrate * 16 * divisor = clock
        // => divisor = clock / (baudrate * 16)
        // => divisor = (clock / 16) / baudrate
        divisor = sp->clock / (16*(setting->baudrate));

        IOWR_ALTERA_16550_UART_DLL(sp->base, (divisor & 0x00FF));
        IOWR_ALTERA_16550_UART_DLH(sp->base, ((divisor >> 8)& 0x00FF));

        /* Clear LCR[7] after program Divisor */
        IOWR_ALTERA_16550_UART_LCR (sp->base, (LCR & ~(ALTERA_16550_UART_LCR_DLAB_MSK)));
    #ifdef ALTERA_16550_UART_USE_IOCTL
        /* Update Device Termios Settings */
        sp->config.baudrate = sp->termios.c_ispeed = sp->termios.c_ospeed = setting->baudrate;
    #endif
    }

    if (sp->config.data_bit != setting->data_bit)
    {
        LCR &= ~(ALTERA_16550_UART_LCR_DLS_MSK);
        LCR |= (setting->data_bit);
        /* Update Device Settings */
        sp->config.data_bit = setting->data_bit;
    }

    if (sp->config.parity_bit != setting->parity_bit)
    {
        if (setting->parity_bit == NO_PARITY)
        {
            LCR &= ~(ALTERA_16550_UART_LCR_PEN_MSK);
         } else
         {
            LCR &= ~(ALTERA_16550_UART_LCR_EPS_MSK);
            LCR |= (((setting->parity_bit)<< 4) | ALTERA_16550_UART_LCR_PEN_MSK);
         }
        /* Update Device Settings */
        sp->config.parity_bit = setting->parity_bit;
    }

    if (sp->config.stop_bit != setting->stop_bit)
    {
        LCR &= ~(ALTERA_16550_UART_LCR_STOP_MSK);
        LCR |= ((setting->stop_bit)<< 2);
        /* Update Device Settings */
        sp->config.stop_bit = setting->stop_bit;
    }
    IOWR_ALTERA_16550_UART_LCR(sp->base, LCR);

    /* FIFO mode configuration
     * hardware FIFO mode must be enable first and
     * user select fifo enabled */
    if (sp->hwfifomode && (setting->fifo_mode))
    {
        regs = ALTERA_16550_UART_FCR_FIFOE_MSK;
        sp->config.fifo_mode = setting->fifo_mode;

        if (sp->config.rx_fifo_level != setting->rx_fifo_level)
        {
            /* Update device setting*/
            sp->config.rx_fifo_level = setting->rx_fifo_level;
        }
        regs &= ~(ALTERA_16550_UART_FCR_RT_MSK);
        regs |= sp->config.rx_fifo_level << 6;

        if (sp->config.tx_fifo_level != setting->tx_fifo_level)
        {
            /* Update device setting*/
            sp->config.tx_fifo_level = setting->tx_fifo_level;
        }
        regs &= ~(ALTERA_16550_UART_FCR_TET_MSK);
        regs |= sp->config.tx_fifo_level << 4;
        /* Program FCR with setting updated */
        IOWR_ALTERA_16550_UART_FCR(sp->base, regs);

        if (sp->config.hwfc != setting->hwfc)
        {
		    regs = IORD_ALTERA_16550_UART_MCR(sp->base);
		    if (setting->hwfc)
			{
                regs |= ALTERA_16550_UART_MCR_RTS_MSK;
                IOWR_ALTERA_16550_UART_MCR(sp->base, regs);

                regs |= ((setting->hwfc)<< 5);
                IOWR_ALTERA_16550_UART_MCR(sp->base, regs);
		    } else
			{
			    /* Disable Autoflow Control */
			    IOWR_ALTERA_16550_UART_MCR(sp->base, (regs & ~(ALTERA_16550_UART_MCR_AFCE_MSK)));
		    }
            /* Update device setting*/
            sp->config.hwfc = setting->hwfc;
        }

    } else
    {
        /* Disable Fifo enable and other level trigger */
        IOWR_ALTERA_16550_UART_FCR(sp->base, 0x0);
        /* Disable HWFC */
        regs = (IORD_ALTERA_16550_UART_MCR(sp->base) &
                ~(ALTERA_16550_UART_MCR_AFCE_MSK));
        IOWR_ALTERA_16550_UART_MCR(sp->base, regs);
       }
    return 0;
}


/*
 * altera_16550_uart_init() is called by the auto-generated function
 * alt_sys_init() in order to initialize a particular instance of this device.
 * It is responsible for configuring the device and associated software
 * constructs.
 */
 void
altera_16550_uart_init(altera_16550_uart_state* sp,
  alt_u32 irq_controller_id,  alt_u32 irq)
{
    extern alt_llist alt_16550_uart_list;
    void* base = sp->base;
    int error;
    alt_u32 regs;

    error = (alt_dev_llist_insert((alt_dev_llist*) sp, &alt_16550_uart_list));
    /* Initialize UART register */
    /* Flush Tx and Rx FIFO */
    IOWR_ALTERA_16550_UART_FCR(base, ALTERA_16550_UART_FCR_FIFOE_MSK);
    IOWR_ALTERA_16550_UART_FCR(base, ALTERA_16550_UART_FCR_FIFOE_MSK |
                                     ALTERA_16550_UART_FCR_FIFOR_MSK |
                                     ALTERA_16550_UART_FCR_XFIFOR_MSK);
    IOWR_ALTERA_16550_UART_FCR(base, 0x0);
    /* Clear any Error status */
    IORD_ALTERA_16550_UART_LSR(sp->base);
    IORD_ALTERA_16550_UART_RBR(sp->base);
    IORD_ALTERA_16550_UART_IIR(sp->base);
    IORD_ALTERA_16550_UART_MSR(sp->base);
    /* Configure default settings */
    IOWR_ALTERA_16550_UART_LCR(base, ((ODD_PARITY << 4) | ALTERA_16550_UART_LCR_PEN_MSK
                                      | (STOPB_1 << 2)| CS_8));
#ifdef ALTERA_16550_UART_USE_IOCTL
    sp->termios.c_cflag = PAODD | PARENB | CSTOPB | CS8;
    sp->termios.c_ispeed = sp->termios.c_ospeed = B115200;
#endif

    /*
     * Initialise the read and write flags and the semaphores used to
     * protect access to the circular buffers when running in a multi-threaded
     * environment.
     */
    error = ALT_FLAG_CREATE (&sp->events, 0)    ||
            ALT_SEM_CREATE (&sp->read_lock, 1)  ||
            ALT_SEM_CREATE (&sp->write_lock, 1);

    if (!error)
    {
        /* register the interrupt handler */
    #ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
        alt_ic_isr_register(irq_controller_id, irq, altera_16550_uart_irq, sp, 0x0);
    #else
        alt_irq_register (irq, sp, altera_16550_uart_irq);
    #endif

	    /* enable interrupts at the device */
        regs = IORD_ALTERA_16550_UART_IER(sp->base);
        regs |= ALTERA_16550_UART_IER_ERBFI_MSK  |
                ALTERA_16550_UART_IER_ETBEI_MSK;
/* MAX */
#if 1
sp->ctrl = regs;
#endif
/* MAX */
        IOWR_ALTERA_16550_UART_IER(base, sp->ctrl);

    }
}

/**
 */
void altera_16550_uart_flush(altera_16550_uart_state* sp)
{
  void* base = sp->base;

  /* Flush Tx and Rx FIFO */
  IOWR_ALTERA_16550_UART_FCR(base, ALTERA_16550_UART_FCR_FIFOE_MSK);
  IOWR_ALTERA_16550_UART_FCR(base, ALTERA_16550_UART_FCR_FIFOE_MSK |
                                   ALTERA_16550_UART_FCR_FIFOR_MSK |
                                   ALTERA_16550_UART_FCR_XFIFOR_MSK);
  IOWR_ALTERA_16550_UART_FCR(base, 0x0);

  /* Clear any Error status */
  IORD_ALTERA_16550_UART_LSR(base);
  IORD_ALTERA_16550_UART_RBR(base);
  IORD_ALTERA_16550_UART_IIR(base);
  IORD_ALTERA_16550_UART_MSR(base);

  /* reset tx and rx buffers */
  sp->tx_start = 0;
  sp->tx_end   = 0;
  sp->rx_start = 0;
  sp->rx_end   = 0;
}
