/*
 * ru.c
 *
 *  Created on: Jun 12, 2018
 *      Author: max
 */

#include <io.h>
#include <alt_types.h>
#include <system.h>

#include "altera_remote_update.h"
//#include "altera_remote_update_regs.h"

/**
 * Remote Update registers definitions
 */
#if 0
#define RU_RECONFIG_TRIGGER_CONDITIONS    (0x00*4)
#define RU_WATCHDOG_TIMEOUT               (0x01*4)
#define RU_WATCHDOG_ENABLE                (0x02*4)
#define RU_PAGE_SELECT                    (0x03*4)
#define RU_CONFIG_MODE                    (0x04*4)
#define RU_RESET_TIMER                    (0x05*4)
#define RU_RECONFIG                       (0x06*4)
#else
#define RU_MASTER_SM_CURRENT_STATE_MODE   (0x00*4)
#define RU_FORCE_EARLY_CONF_DONE          (0x04*4)
#define RU_WATCHDOG_TIMEOUT               (0x08*4)
#define RU_WATCHDOG_ENABLE                (0x0C*4)
#define RU_BOOT_ADDRESS                   (0x10*4)
#define RU_FORCE_INTERNAL_OSC             (0x14*4)
#define RU_RECONFIG_TRIGGER_CONDITIONS    (0x18*4)
#define RU_RESET_TIMER                    (0x1C*4)
#define RU_RECONFIG                       (0x1D*4)
#endif


/**
 *
 */
alt_u32 ruLoad(alt_u32 addr)
{
#if 0
	/* Turn off watchdog */
	IOWR_32DIRECT(REMOTE_UPDATE_0_BASE, RU_WATCHDOG_ENABLE, 0);
	/* set start address of configuration image */
	IOWR_32DIRECT(REMOTE_UPDATE_0_BASE, RU_PAGE_SELECT, addr);
	/* Trigger reconfiguration */
	IOWR_32DIRECT(REMOTE_UPDATE_0_BASE, RU_RECONFIG, 1);
#endif
#if 0
	/* cyclone IV */
	/* Turn off watchdog */
	IOWR_32DIRECT(REMOTE_UPDATE_0_BASE, RU_WATCHDOG_ENABLE, 0);
	/* set start address of configuration image */
	IOWR_32DIRECT(REMOTE_UPDATE_0_BASE, RU_BOOT_ADDRESS, addr);
	/* Trigger reconfiguration */
	IOWR_32DIRECT(REMOTE_UPDATE_0_BASE, RU_RECONFIG, 1);
#endif
#if 0
	altera_remote_update_state *sp;
	alt_u32 mode;
	alt_u32 conf;
	alt_u32 wdt = 0;

	sp = altera_remote_update_open(REMOTE_UPDATE_0_NAME);
	altera_remote_update_init(sp);
	mode = IORD_ALTERA_RU_CONFIG_MODE(sp->base);
	conf = IORD_ALTERA_RU_RECONFIG_TRIGGER_CONDITIONS(sp->base);
//?????	addr >>= 2;
	if ((mode == 0) && (conf == 0)) {
		/*altera_remote_update_trigger_reconfig(sp, APPLICATION, addr, wdt);*/
		/* Configure watchdog timer */
		IOWR_ALTERA_RU_WATCHDOG_ENABLE(sp->base, ~ALTERA_RU_WATCHDOG_ENABLE);
		/* Configure boot address */
		IOWR_ALTERA_RU_PAGE_SELECT(sp->base, addr);
		/* Configure reconfig mode */
		mode = APPLICATION;
		IOWR_ALTERA_RU_CONFIG_MODE(sp->base, mode & ALTERA_RU_RECONFIG_MODE_MASK);
		/* Trigger reconfig */
		IOWR_ALTERA_RU_RECONFIG(sp->base, 0x1);
	}
#endif

#if 1
	alt_u32 remote_update_state;
	alt_u32 remote_update_address;

	remote_update_state = altera_remote_update_master_sm_current_state_mode(REMOTE_UPDATE_0_BASE, 0);
	remote_update_address = altera_remote_update_boot_address(REMOTE_UPDATE_0_BASE, 1);
	if ( (remote_update_state == 0) && (remote_update_address==0) ) {
		altera_remote_update_trigger_reconfig(REMOTE_UPDATE_0_BASE, APPLICATION, addr, 0);
	}
#endif
	/* Wait briefly until configuration takes place */
    while (1);

	return 0;
}

#if 0



#define ALT_MODULE_CLASS_remote_update_0 altera_remote_update
#define REMOTE_UPDATE_0_BASE 0x70010000
#define REMOTE_UPDATE_0_IRQ -1
#define REMOTE_UPDATE_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define REMOTE_UPDATE_0_NAME "/dev/remote_update_0"
#define REMOTE_UPDATE_0_SPAN 128
#define REMOTE_UPDATE_0_TYPE "altera_remote_update"




/**
 * http://billauer.co.il/blog/2017/06/remote-update-intel-fpga-cyclone-iv/
 */
void do_remote_update(void)
{
  alt_u32 app_bitstream_addr = 0x100000;
  alt_u32 mode = IORD_32DIRECT(REMOTE_UPDATE_0_BASE, 0) & 3;
  alt_u32 config_reason = IORD_32DIRECT(REMOTE_UPDATE_0_BASE, 0x64);

  if ((mode == 0) && (config_reason == 0)) {
    IOWR_32DIRECT(REMOTE_UPDATE_0_BASE, 0x30, 0); // Turn off watchdog
    IOWR_32DIRECT(REMOTE_UPDATE_0_BASE, 0x40, app_bitstream_addr);
    IOWR_32DIRECT(REMOTE_UPDATE_0_BASE, 0x74, 1); // Trigger reconfiguration
    while (1); // Wait briefly until configuration takes place
  }
}

#endif
