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

#ifndef __ALT_REMOTE_UPDATE_H__
#define __ALT_REMOTE_UPDATE_H__

#include "alt_types.h"
#include "sys/alt_llist.h"
#include "priv/alt_dev_llist.h"
#include "sys/alt_dev.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef enum reconfig_mode
{
	FACTORY = 0,
	APPLICATION
} mode;

/*
    Public API
*/
int altera_remote_update_trigger_reconfig(
		    alt_u32 base,
			alt_u32 mode,
			alt_u32 boot_address_value,
			alt_u32 watch_dog_value
		);

alt_u32 altera_remote_update_master_sm_current_state_mode (alt_u32 base, alt_u8 source);

alt_u32 altera_remote_update_boot_address (alt_u32 base, alt_u8 source);

alt_u32 altera_remote_update_trigger_reconfig_condition (alt_u32 base);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ALT_REMOTE_UPDATE_H__ */
