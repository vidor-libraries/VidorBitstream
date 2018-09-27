#include "VidorIP.h"
#include "VidorMailbox.h"

int VidorIP::version() {
	uint32_t  rpc[1];

	rpc[0] = RPC_CMD(info.giid, 0, 1);
	return VidorMailbox.sendCommand(rpc, 1);
}

int VidorIP::deinit() {
	uint32_t  rpc[1];

	rpc[0] = RPC_CMD(info.giid, info.chn, 4);
	return VidorMailbox.sendCommand(rpc, 1);
}

extern LinkedList<VidorIP*> IPList;

void VidorIP::addToList() {
	IPList.add(this);
};