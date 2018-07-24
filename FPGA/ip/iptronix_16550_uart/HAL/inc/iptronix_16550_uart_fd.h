/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2007 Altera Corporation, San Jose, California, USA.           *
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

#ifndef __ALT_16550_UART_FD_H__
#define __ALT_16550_UART_FD_H__

#include "sys/alt_dev.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*
 * Externally referenced routines
 */
extern int altera_16550_uart_read_fd (alt_fd* fd, alt_u8* ptr, int len);
extern int altera_16550_uart_write_fd (alt_fd* fd, const alt_u8* ptr, int len);

/*
 * Device structure definition. This is needed by alt_sys_init in order to
 * reserve memory for the device instance.
 */
typedef struct altera_16550_uart_dev_s
{
  alt_dev                 dev;
  altera_16550_uart_state state;
} altera_16550_uart_dev;

extern int altera_16550_uart_ioctl_fd (alt_fd* fd, int req, void* arg);
extern int altera_16550_uart_close_fd(alt_fd* fd);

#ifdef ALTERA_16550_UART_USE_IOCTL
#define ALTERA_16550_UART_IOCTL_FD altera_16550_uart_ioctl_fd
#else
#define ALTERA_16550_UART_IOCTL_FD NULL
#endif //#ifdef ALTERA_16550_UART_USE_IOCTL

#define ALTERA_16550_UART_DEV_INSTANCE(name, d)       \
  static altera_16550_uart_dev d =                     \
   {                                                   \
     {                                                 \
       ALT_LLIST_ENTRY,                                \
       name##_NAME,                                    \
       NULL, /* open */                                \
       altera_16550_uart_close_fd,                     \
       altera_16550_uart_read_fd,                      \
       altera_16550_uart_write_fd,                     \
       NULL, /* lseek */                               \
       NULL, /* fstat */                               \
       ALTERA_16550_UART_IOCTL_FD,                    \
     },    \
     {\
      ALT_LLIST_ENTRY,                                \
      name##_BASE,                 \
      name##_FREQ,                 \
      name##_FIFO_MODE,            \
     }\
   }

#define ALTERA_16550_UART_DEV_INIT(name, d)           \
  {                                                    \
    ALTERA_16550_UART_STATE_INIT(name, d.state);      \
                                                       \
    /* make the device available to the system */      \
    alt_dev_reg(&d.dev);                               \
  }

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ALT_16550_UART_FD_H__ */
