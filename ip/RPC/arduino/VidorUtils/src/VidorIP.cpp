#include "VidorIP.h"
#include "VidorMailbox.h"
#include "VidorUtils.h"

int VidorIP::version() {
	uint32_t  rpc[1];

	rpc[0] = RPC_CMD(giid, 0, 1);
	return VidorMailbox.sendCommand(rpc, 1);
}

int VidorIP::init(int uid, uint16_t pins...) {

	// MUST be called after FPGA.init()
	this->uid = uid;
	uint32_t rpc[1];

	// scan list and replace self with matching uid 
	LinkedList<VidorIP*> list = VidorUtils::IPList;
	for (int i=0; i < list.size(); i++) {
		if (list.get(i)->uid == uid) {
			// ask for matching pins
			for (int c = 0; c < list.get(i)->availableChannels; c++) {
				rpc[0] = RPC_CMD(giid, c, 3);
				int ret = VidorMailbox.sendCommand(rpc, 1);
				if (ret==-1) {
					break;
				}
				uint32_t* discoverPins = (uint32_t*)malloc(ret * sizeof(uint32_t));

				VidorMailbox.read(2, discoverPins, ret);

				va_list args;
				va_start(args, pins);

				int j;
				for (j = 0; j < ret; j++) {
					int pin = va_arg(args, uint16_t);
					// search pin in discoverPins
					if (!bufContains(discoverPins, pin, ret)) {
						break;
					}
				}
				va_end(args);

				free(discoverPins);

				if (ret != j) {
					continue;
				}
				// we got a gid!
				gid = i;
				chn = c;
				list.remove(i);
				list.add(this);
				return 1;
			}
		}
	}
	return -1;
}

int VidorIP::deinit() {
	uint32_t  rpc[1];

	rpc[0] = RPC_CMD(giid, chn, 4);
	return VidorMailbox.sendCommand(rpc, 1);
}