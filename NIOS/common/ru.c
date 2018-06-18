/*
 * ru.c
 *
 *  Created on: Jun 12, 2018
 *      Author: max
 */

#include <alt_types.h>
#include <system.h>

#include "altera_remote_update.h"

/**
 *
 */
alt_u32 ruLoad(alt_u32 addr)
{
  alt_u32 remote_update_state;
  alt_u32 remote_update_address;

  remote_update_state = altera_remote_update_master_sm_current_state_mode(REMOTE_UPDATE_0_BASE, 0);
  remote_update_address = altera_remote_update_boot_address(REMOTE_UPDATE_0_BASE, 1);
  if ( (remote_update_state == 0) && (remote_update_address==0) ) {
    altera_remote_update_trigger_reconfig(REMOTE_UPDATE_0_BASE, APPLICATION, addr, 0);
  }

  /* Wait briefly until configuration takes place */
  while (1);

  return 0;
}
