#ifndef __VIDOR_LIB__
#define __VIDOR_LIB__

#define THIS_BITSTREAM_CHECKSUM 		"45c83d4fb44010509dc529cd8a8633e7"

#if defined(FPGA_BITSTREAM_CHECKSUM)
#error "Cannot load more than one bitstream"
#endif

#if !defined(FPGA_BITSTREAM_CHECKSUM)
#define FPGA_BITSTREAM_CHECKSUM 	THIS_BITSTREAM_CHECKSUM
#endif

#include "VidorUtils.h"
#include "defines.h"

class VidorBase : public VidorUtils {
public:

	VidorBase() {}

	static void onInterrupt();

	int begin(bool jumpToApp = true) {
		int ret = VidorUtils::begin(jumpToApp);

		attachInterrupt(IRQ_PIN, VidorBase::onInterrupt, FALLING);

		if (jumpToApp) {
			// wait one second to make sure jump was ok
			delay(1000);
		}

		return ret;
	}

	bool ready() {
		return (version() != 0) &&  (version() != 0xFFFFFFFF);
	}

	uint32_t version() {
		uint32_t ptr[1];
		uint32_t ver;

		ptr[0] = 0 | 1;
		ver = mbCmdSend(ptr, 1);
		return ver;
	}

	uint32_t printConfig(void)
	{
		uint32_t ptr[1];
		sFpgaCfg cfg;
		char str[64];

		ptr[0] = 0 | 2;
		mbCmdSend(ptr, 1);
		mbRead(1, &cfg, (sizeof(cfg)+3)/4);

		int i;
		sprintf(str, "number of devices %d", cfg.num);
		Serial.println(str);
		for(i=0; i<cfg.num; i++){
			sprintf(str, "%d %08X %s", cfg.dev[i].num, cfg.dev[i].cod, configToString(cfg.dev[i].cod));
			Serial.println(str);
		}
		return 0;
	}
};

extern VidorBase VD;

#endif
