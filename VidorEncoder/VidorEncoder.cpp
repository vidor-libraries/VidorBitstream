#include "VidorEncoder.h"

VidorEncoder::VidorEncoder(int index){
	idx = index;
}

void VidorEncoder::write(int32_t p){
	offset = read() - p;
}

int32_t VidorEncoder::read(){
	uint32_t ptr[1];
	ptr[0] = MB_DEV_ENC | ((idx & 0x0F)<<20) | 1;
	return (mbCmdSend(ptr, 1) - offset);
}
