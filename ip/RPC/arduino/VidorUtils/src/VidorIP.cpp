#include "VidorIP.h"
#include "VidorMailbox.h"
#include "VidorUtils.h"

int VidorIP::version() {
	uint32_t  rpc[1];

	rpc[0] = RPC_CMD(info.giid, 0, 1);
	return VidorMailbox.sendCommand(rpc, 1);
}

int VidorIP::init(int uid, ...) {

	// MUST be called after FPGA.init()
	this->info.uid = uid;
	uint32_t rpc[1];

	va_list args;
	va_start(args, uid);
	// scan list and replace self with matching uid 
	int ret = VidorUtils::discover(&info, args);
	va_end(args);

	if (ret != -1) {
		// TODO: here a call should exist to multiplex and lock pins for a specific giid/chn
		// rpc[0] = RPC_CMD(info.giid, info.chn, 4);
		// int ret = VidorMailbox.sendCommand(rpc, 1);
		VidorUtils::addToList(static_cast<VidorIP *>(this));
	}

	return ret;
}

int VidorIP::deinit() {
	uint32_t  rpc[1];

	rpc[0] = RPC_CMD(info.giid, info.chn, 4);
	return VidorMailbox.sendCommand(rpc, 1);
}