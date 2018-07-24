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

#include "sys/alt_dev.h"
#include "sys/alt_irq.h"
#include "sys/ioctl.h"
#include "sys/alt_errno.h"

#include "iptronix_16550_uart_regs.h"
#include "iptronix_16550_uart.h"

/*
 * altera_16550_uart_write() is called by the system write() function in order
 * to write a block of data to the UART. "len" is the length of the data to
 * write, and "ptr" indicates the source address. "sp" is the state pointer
 * for the device to be written to.
 *
 * Permission checks are made before the call to altera_16550_uart_write(), so
 * we know that the file descriptor has been opened with the correct permissions
 * for this operation.
 *
 * The return value is the number of 2 bytes(because supporting CS_9) actually
 * written.
 *
 * This function does not communicate with the device directly. Instead data is
 * transfered to a circular buffer. The interrupt handler is then responsible
 * for copying data from this buffer into the device.
 */

int
altera_16550_uart_write(altera_16550_uart_state* sp, const alt_u8* ptr, int len,
  int flags)
{
    alt_irq_context context;
    int             no_block;
    alt_u32         next;
    int             count = len;

    sp->ctrl = IORD_ALTERA_16550_UART_IER(sp->base);
    /*
     * Construct a flag to indicate whether the device is being accessed in
     * blocking or non-blocking mode.
     */
    no_block = (flags & O_NONBLOCK);

    /*
     * When running in a multi threaded environment, obtain the "write_lock"
     * semaphore. This ensures that writing to the device is thread-safe.
     */
    ALT_SEM_PEND (sp->write_lock, 0);

    /*
     * Loop transferring data from the input buffer to the transmit circular
     * buffer. The loop is terminated once all the data has been transferred,
     * or, (if in non-blocking mode) the buffer becomes full.
     */
    while (count)
    {
        /* Determine the next slot in the buffer to access */
        next = (sp->tx_end + 1) & ALT_16550_UART_BUF_MSK;

        /* block waiting for space if necessary */
        if (next == sp->tx_start)
        {
            if (no_block)
            {
                /* Set errno to indicate why this function returned early */
                ALT_ERRNO = EWOULDBLOCK;
                break;
            } else
            {
                /* Block waiting for space in the circular buffer */
                /* First, ensure transmit interrupts are enabled to avoid deadlock */
                context = alt_irq_disable_all ();
                sp->ctrl |= (ALTERA_16550_UART_IER_ETBEI_MSK);
                IOWR_ALTERA_16550_UART_IER(sp->base, sp->ctrl);
                alt_irq_enable_all (context);
                /* wait for space to come free */
                do
                {
                    /*
                     * When running in a multi-threaded mode, we pend on the write event
                     * flag set in the interrupt service routine. This avoids wasting CPU
                     * cycles waiting in this thread, when we could be doing something
                     * more profitable elsewhere.
                     */
                    ALT_FLAG_PEND (sp->events,
                                   ALT_16550_UART_WRITE_RDY,
                                   OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME,
                                   0);
                } while ((next == sp->tx_start));
            }
        }
        count--;
        /* Add the next character to the transmit buffer */
        sp->tx_buf[sp->tx_end] = *ptr++;
        sp->tx_end = next;
    }
    /*
     * Now that access to the circular buffer is complete, release the write
     * semaphore so that other threads can access the buffer.
     */
    ALT_SEM_POST (sp->write_lock);
    /*
     * Ensure that interrupts are enabled, so that the circular buffer can
     * drain.
     */
    context = alt_irq_disable_all ();
    sp->ctrl |= ALTERA_16550_UART_IER_ETBEI_MSK ;
    IOWR_ALTERA_16550_UART_IER(sp->base, sp->ctrl);
    alt_irq_enable_all (context);
    /* return the number of bytes written */
    return (len - count);
}
