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

#ifndef __ALT_16550_UART_H__
#define __ALT_16550_UART_H__

#include <stddef.h>
#include <sys/termios.h>
#include <wchar.h>
#include "sys/alt_warning.h"
#include "os/alt_sem.h"
#include "os/alt_flag.h"
#include "alt_types.h"
#include "priv/alt_dev_llist.h"
#include "sys/alt_dev.h"
#include "priv/alt_file.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*
 * ALT_16550_UART_READ_RDY and ALT_16550_UART_WRITE_RDY are the bitmasks
 * that define uC/OS-II event flags that are releated to this device.
 *
 * ALT_16550_UART_READY_RDY indicates that there is read data in the buffer
 * ready to be processed. ALT_16550_UART_WRITE_RDY indicates that the transmitter is
 * ready for more data.
 */
#define ALT_16550_UART_READ_RDY  0x1
#define ALT_16550_UART_WRITE_RDY 0x2

/*
 * ALT_16550_UART_BUF_LEN is the length of the software circular buffers used
 * to hold pending transmit and receive data.This value must be a power of two.
 * Since the maximun size of HW FIFO available to set is 128, so the ring buffer
 * size need bigger than HW FIFO size.
 */
#define ALT_16550_UART_BUF_LEN (64)

/*
 * ALT_16550_UART_BUF_MSK is used as an internal convenience for detecting
 * the end of the arrays used to implement the transmit and receive buffers.
 */
#define ALT_16550_UART_BUF_MSK (ALT_16550_UART_BUF_LEN - 1)
/*
 * The UartConfig structure is used to hold device specific data.
 * This includes the transmit and receive buffers.
 *
 * An instance of this structure is created in the auto-generated
 * alt_sys_init.c file for each UART listed in the systems SOPCINFO file. This is
 * done using the ALTERA_16550_UART_STATE_INSTANCE macro given below.
 */

typedef enum stopbit      { STOPB_1 = 0,STOPB_2 } StopBit;
typedef enum paritybit    { ODD_PARITY = 0, EVEN_PARITY, MARK_PARITY, SPACE_PARITY, NO_PARITY } ParityBit;
typedef enum databit      { CS_5 = 0, CS_6, CS_7, CS_8, CS_9 = 256} DataBit;
typedef enum baud
{
    BR9600 = B9600,
    BR19200 = B19200,
    BR38400 = B38400,
    BR57600 = B57600,
    BR115200 = B115200
} Baud;
typedef enum rx_fifo_level_e { RXONECHAR = 0, RXQUARTER, RXHALF, RXFULL } Rx_FifoLvl;
typedef enum tx_fifo_level_e { TXEMPTY = 0, TXTWOCHAR, TXQUARTER, TXHALF } Tx_FifoLvl;
typedef struct uart_config_s
{
    StopBit    stop_bit;
    ParityBit  parity_bit;
    DataBit    data_bit;
    Baud       baudrate;
    alt_u32    fifo_mode;
    Rx_FifoLvl rx_fifo_level;
    Tx_FifoLvl tx_fifo_level;
    alt_u32    hwfc;
} UartConfig;
typedef struct LSR_s
{
  alt_u8	dr:1;
  alt_u8	oe:1;
  alt_u8	pe:1;
  alt_u8	fe:1;
  alt_u8	bi:1;
  alt_u8	thre:1;
  alt_u8	temt:1;
  alt_u8	rfe:1;
}line_status_reg;
typedef struct altera_16550_uart_state_s
{
 alt_dev               dev;
 void*                 base;                 /* The base address of the device */
 alt_u32               clock;
 alt_u32               hwfifomode;
 alt_u32               ctrl;                 /* Shadow value of the LSR register */
 volatile alt_u32      rx_start;             /* Start of the pending receive data */
 volatile alt_u32      rx_end;               /* End of the pending receive data */
 volatile alt_u32      tx_start;             /* Start of the pending transmit data */
 volatile alt_u32      tx_end;               /* End of the pending transmit data */
 alt_u32               freq;                 /* Current clock freq rate */
 UartConfig            config;               /* Uart setting */
 #ifdef ALTERA_16550_UART_USE_IOCTL
 struct termios        termios;
 #endif
 alt_u32               flags;                /* Configuration flags */
 ALT_FLAG_GRP         (events)               /* Event flags used for
                                              * foreground/background in mult-threaded
                                              * mode */
 ALT_SEM              (read_lock)            /* Semaphore used to control access to the
                                              * read buffer in multi-threaded mode */
 ALT_SEM              (write_lock)           /* Semaphore used to control access to the
                                              * write buffer in multi-threaded mode */
volatile alt_u8      rx_buf[ALT_16550_UART_BUF_LEN]; /* The receive buffer */
volatile alt_u8      tx_buf[ALT_16550_UART_BUF_LEN]; /* The transmit buffer */
line_status_reg	      line_status; /* line register status for the current read byte data of RBR or data at the top of FIFO*/
alt_u8                error_ignore; /* received data will be discarded
for the current read byte data of RBR or data at the top of FIFO if pe, fe and bi errors detected after error_ignore is set to '0' */
} altera_16550_uart_state;

alt_u32 alt_16550_uart_config(altera_16550_uart_state* sp, UartConfig *setting);
int altera_16550_uart_close(altera_16550_uart_state* sp, int flags);
altera_16550_uart_state* altera_16550_uart_open (const char *name);
int altera_16550_uart_read(altera_16550_uart_state* sp, alt_u8* ptr, int len, int flags);
int altera_16550_uart_write(altera_16550_uart_state* sp, const alt_u8* ptr, int len, int flags);
void altera_16550_uart_flush(altera_16550_uart_state* sp);

/*
 * The macro ALTERA_16550_UART_INSTANCE is used by the auto-generated file
 * alt_sys_init.c to create an instance of this device driver state.
 */


typedef int (*ALTERA_16550_UART_OPEN)  (alt_fd* fd, const char* name, int flags, int mode);
typedef int (*ALTERA_16550_UART_CLOSE) (alt_fd* fd);

#define IPTRONIX_16550_UART_STATE_INSTANCE(name, state) \
  altera_16550_uart_state state =            \
{                                   \
 {                                  \
       ALT_LLIST_ENTRY,             \
       name##_NAME,                 \
       ((ALTERA_16550_UART_OPEN)(altera_16550_uart_open)),      \
       ((ALTERA_16550_UART_CLOSE)(altera_16550_uart_close)),     \
       NULL,                        \
       NULL,                        \
       NULL,                        \
       NULL,                        \
       NULL,                        \
 },                                 \
       ((void *)(name##_BASE)),     \
       name##_FREQ,                 \
       name##_FIFO_MODE,            \
}
/*
 * altera_avalon_uart_init() is called by the auto-generated function
 * alt_sys_init() for each UART in the system. This is done using the
 * ALTERA_16550_UART_INIT macro given below.
 *
 * This function is responsible for performing all the run time initilisation
 * for a device instance, i.e. registering the interrupt handler, and
 * regestering the device with the system.
 */
extern void altera_16550_uart_init(altera_16550_uart_state* sp,
 alt_u32 irq_controller_id, alt_u32 irq);

/*
 * The macro ALTERA_16550_UART_STATE_INIT is used by the auto-generated file
 * alt_sys_init.c to initialize an instance of the device driver state.
 *
 * This macro performs a sanity check to ensure that the interrupt has been
 * connected for this device. If not, then an apropriate error message is
 * generated at build time.
 */

#define IPTRONIX_16550_UART_STATE_INIT(name, state)                    \
 if (name##_IRQ == ALT_IRQ_NOT_CONNECTED)                            \
 {                                                                   \
  ALT_LINK_ERROR ("Error: Interrupt not connected for " #name ". "   \
          "You have selected the interrupt driven version of "       \
          "the ALTERA 16550 UART driver, but the interrupt is "     \
          "not connected for this device. You can select a "         \
          "polled mode driver by checking the 'small driver' "       \
          "option in the HAL configuration window, or by "           \
          "using the -DALTERA_AVALON_UART_SMALL preprocessor "       \
          "flag.");                                                  \
 }                                                                   \
 else                                                                \
 {                                                                   \
  altera_16550_uart_init(&state, name##_IRQ_INTERRUPT_CONTROLLER_ID, \
   name##_IRQ);                                                      \
 }

/*
 * Include in case non-direct version of driver required.
 */
#include "iptronix_16550_uart_fd.h"

#ifdef IPTRONIX_16550_UART_USE_IOCTL

#define IPTRONIX_16550_UART_INSTANCE(name, dev) \
   IPTRONIX_16550_UART_DEV_INSTANCE(name, dev)
#define IPTRONIX_16550_UART_INIT(name, dev) \
   IPTRONIX_16550_UART_DEV_INIT(name, dev)

#else /* !ALTERA_16550_UART_USE_IOCTL */

#define IPTRONIX_16550_UART_INSTANCE(name, state) \
   IPTRONIX_16550_UART_STATE_INSTANCE(name, state)
#define IPTRONIX_16550_UART_INIT(name, state) \
   IPTRONIX_16550_UART_STATE_INIT(name, state)

#endif /* ALTERA_16550_UART_USE_IOCTL */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ALT_AVALON_UART_H__ */
