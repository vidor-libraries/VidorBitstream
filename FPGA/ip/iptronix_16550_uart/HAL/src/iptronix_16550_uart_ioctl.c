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
#include <string.h>

#include "sys/alt_irq.h"
#include "sys/ioctl.h"
#include "sys/alt_errno.h"

#include "iptronix_16550_uart_regs.h"
#include "iptronix_16550_uart.h"

#ifdef ALTERA_16550_UART_USE_IOCTL
/*
 * altera_16550_uart_ioctl() is called by the system ioctl() function to handle
 * ioctl requests for the UART. The only ioctl requests supported are TIOCMGET
 * and TIOCMSET.
 *
 * TIOCMGET returns a termios structure that describes the current device
 * configuration.
 *
 * TIOCMSET sets the device (if possible) to match the requested configuration.
 * The requested configuration is described using a termios structure passed
 * through the input argument "arg".
 */

static int altera_16550_uart_tiocmget(altera_16550_uart_state* sp,
  struct termios* term);
static int altera_16550_uart_tiocmset(altera_16550_uart_state* sp,
  struct termios* term);

int 
altera_16550_uart_ioctl(altera_16550_uart_state* sp, int req, void* arg)
{
  int rc = -ENOTTY;

  switch (req)
  {
  case TIOCMGET:
    rc = altera_16550_uart_tiocmget(sp, (struct termios*) arg);
    break;
  case TIOCMSET:
    rc = altera_16550_uart_tiocmset(sp, (struct termios*) arg);
    break;
  default:
    break;
  }
  return rc;
}

/*
 * altera_16550_uart_tiocmget() is used by altera_16550_uart_ioctl() to fill
 * in the input termios structure with the current device configuration. 
 *
 * See termios.h for further details on the contents of the termios structure.
 */

static int 
altera_16550_uart_tiocmget(altera_16550_uart_state* sp,
  struct termios* term)
{
  memcpy (term, &sp->termios, sizeof (struct termios));
  return 0;
}

/*
 * altera_16550_uart_tiocmset() is used by altera16550_uart_ioctl() to 
 * configure the device according to the settings in the input termios 
 * structure. In practice the only configuration that can be changed is the
 * baud rate, and then only if the hardware is configured to have a writable
 * baud register.
 */

static int 
altera_16550_uart_tiocmset(altera_16550_uart_state* sp,
  struct termios* term)
{
    speed_t speed;
    alt_u8  stop_bit, parity, data_bit;
    alt_u32 divisor;
    UartConfig *UART_Config = malloc(1*sizeof(UartConfig));
  
    
    stop_bit = (term->c_cflag & CSTOPB) ? STOPB_2 : STOPB_1;
    parity = (term->c_cflag & PARENB) ? ((term->c_cflag & PAODD) ? 
              ODD_PARITY : EVEN_PARITY) : NO_PARITY;
    switch (term->c_cflag & CSIZE)
    {
        case CS5:
            data_bit = CS_5;
            break;
        case CS6:
            data_bit = CS_6;
            break;
        case CS7:
            data_bit = CS_7;
            break;
        default:
            data_bit = CS_8;
            break;
    }
  
    speed = term->c_ispeed;

    UART_Config->fifo_mode = sp->config.fifo_mode;
    UART_Config->hwfc = sp->config.hwfc;
    UART_Config->rx_fifo_level = sp->config.rx_fifo_level;
    UART_Config->tx_fifo_level = sp->config.tx_fifo_level;
    UART_Config->baudrate   = speed;
    UART_Config->stop_bit   = stop_bit;
    UART_Config->parity_bit = parity;
    UART_Config->data_bit   = data_bit;
    alt_16550_uart_config ( sp, UART_Config);
    /* Update Device Settings */
    sp->termios.c_cflag = term->c_cflag;
    /*
     * If the request was for an unsupported setting, return an error.
     */
    if (memcmp(term, &sp->termios, sizeof (struct termios)))
    {
        sp->termios.c_ispeed = sp->termios.c_ospeed = speed;
        return -EIO;
    }
  return 0;
}
#endif /* ALTERA_16550_UART_USE_IOCTL */