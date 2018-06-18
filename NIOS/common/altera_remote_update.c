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
*                                                                             *
******************************************************************************/
#include <errno.h>
#include <io.h>
#include <string.h>
#include "sys/param.h"
#include "alt_types.h"
#include "sys/alt_debug.h"
#include "sys/alt_cache.h"
#include "altera_remote_update.h"
#include "altera_remote_update_regs.h"

alt_u32 altera_remote_update_master_sm_current_state_mode (alt_u32 base, alt_u8 source)
{
  source &= 0x3;
  return IORD_ALTERA_RU_MASTER_SM_CURRENT_STATE_MODE(base + source);
}

alt_u32 altera_remote_update_trigger_reconfig_condition (alt_u32 base)
{
  return IORD_ALTERA_RU_RECONFIG_TRIGGER_CONDITIONS(base);
}

alt_u32 altera_remote_update_boot_address (alt_u32 base, alt_u8 source)
{
  return IORD_ALTERA_RU_PAGE_SELECT(base + source);
}

/*
 * altera_remote_update_trigger_reconfig
 * The return value will be 0 on success, and non-zero otherwise.
 */
int altera_remote_update_trigger_reconfig (alt_u32 base,
  alt_u32 mode, alt_u32 boot_address, alt_u32 watch_dog_timeout)
{
  /* Configure watchdog timer */
  if( watch_dog_timeout > 0 ) {
    IOWR_ALTERA_RU_WATCHDOG_ENABLE(base, ALTERA_RU_WATCHDOG_ENABLE);
    IOWR_ALTERA_RU_WATCHDOG_TIMEOUT(base, watch_dog_timeout & ALTERA_RU_WATCHDOG_TIMEOUT_WRITE_MASK);
  } else {
    IOWR_ALTERA_RU_WATCHDOG_ENABLE(base, ~ALTERA_RU_WATCHDOG_ENABLE);
  }
  /* Configure boot address */
  IOWR_ALTERA_RU_PAGE_SELECT(base, boot_address);
  /* Configure reconfig mode */
  IOWR_ALTERA_RU_CONFIG_MODE(base, mode & ALTERA_RU_RECONFIG_MODE_MASK);
  /* Trigger reconfig */
  IOWR_ALTERA_RU_RECONFIG(base, 0x1);
  return 0;
}
