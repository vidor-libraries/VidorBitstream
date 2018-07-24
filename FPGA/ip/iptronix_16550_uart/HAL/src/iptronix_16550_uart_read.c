/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2006 Altera Corporation, San Jose, California, USA.           *
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

/*
 * altera_16550_uart_read() is called by the system read() function in order to
 * read a block of data from the UART. "len" is the maximum length of the data
 * to read, and "ptr" indicates the destination address. "sp" is the state
 * pointer for the device to be read from.
 *
 * Permission checks are made before the call to altera_16550_uart_read(), so
 * we know that the file descriptor has been opened with the correct permissions
 * for this operation.
 *
 * The return value is the number of 2 bytes(because supporting CS_9) actually
 * read.
 *
 * This function does not communicate with the device directly. Instead data is
 * transfered from a circular buffer. The interrupt handler is then responsible
 * for copying data from the device into this buffer.
 */

int
altera_16550_uart_read(altera_16550_uart_state* sp, alt_u8* ptr, int len,
  int flags)
{
	alt_irq_context context;
	int             block;
	//alt_u32         next;
	alt_u8          read_would_block = 0;
	int             count = 0;

	sp->ctrl = IORD_ALTERA_16550_UART_IER(sp->base);
	/*
     * Construct a flag to indicate whether the device is being accessed in
     * blocking or non-blocking mode.
     */
	block = !(flags & O_NONBLOCK);

	/*
     * When running in a multi threaded environment, obtain the "read_lock"
     * semaphore. This ensures that reading from the device is thread-safe.
     */
	ALT_SEM_PEND (sp->read_lock, 0);

	/*
     * Calculate which slot in the circular buffer is the next one to read
     * data from.
     */
	//next = (sp->rx_start + 1) & ALT_16550_UART_BUF_MSK;

	/*
     * Loop, copying data from the circular buffer to the destination address
     * supplied in "ptr". This loop is terminated when the required number of
     * bytes have been read. If the circular buffer is empty, and no data has
     * been read, then the loop will block (when in blocking mode).
     *
     * If the circular buffer is empty, and some data has already been
     * transferred, or the device is being accessed in non-blocking mode, then
     * the loop terminates without necessarily reading all the requested data.
     */
	do
	{
		/*
		 * Read the required amount of data, until the circular buffer runs
         * empty
         */
		while ((count < len) && (sp->rx_start != sp->rx_end))
		{
			count++;
			*ptr++ = sp->rx_buf[sp->rx_start];

			sp->rx_start = (++sp->rx_start) & ALT_16550_UART_BUF_MSK;
		}
		/*
         * If no data has been transferred, the circular buffer is empty, and
         * this is not a non-blocking access, block waiting for data to arrive.
         */
		if (!count && (sp->rx_start == sp->rx_end))
		{
			if (!block)
			{
				/* Set errno to indicate the reason we're not returning any data */
				ALT_ERRNO = EWOULDBLOCK;
				read_would_block = 1;
				break;
			} else
			{
				/* Block waiting for some data to arrive */
				/* First, ensure read interrupts are enabled to avoid deadlock */
				context = alt_irq_disable_all ();
				sp->ctrl |= ALTERA_16550_UART_IER_ERBFI_MSK;
				IOWR_ALTERA_16550_UART_IER(sp->base, sp->ctrl);
				alt_irq_enable_all (context);

				/*
                 * When running in a multi-threaded mode, we pend on the read event
                 * flag set in the interrupt service routine. This avoids wasting CPU
                 * cycles waiting in this thread, when we could be doing something more
                 * profitable elsewhere.
                 */
				ALT_FLAG_PEND (sp->events,
						       ALT_16550_UART_READ_RDY,
						       OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME,
						       0);
			}
		}
	}
	while (!count && len);
	/*
	 * Now that access to the circular buffer is complete, release the read
     * semaphore so that other threads can access the buffer.
     */
    ALT_SEM_POST (sp->read_lock);
    /*
     * Ensure that interrupts are enabled, so that the circular buffer can
     * re-fill.
     */
    context = alt_irq_disable_all ();
    sp->ctrl |= ALTERA_16550_UART_IER_ERBFI_MSK;
    IOWR_ALTERA_16550_UART_IER(sp->base, sp->ctrl);
    alt_irq_enable_all (context);
    /* Return the number of bytes read */
    if(read_would_block)
    {
    	return ~EWOULDBLOCK;
    }else
    {
    	return count;
    }
}

