#include "VidorIRQ.h"
#include "VidorMailbox.h"
#include "VidorIP.h"

#define MB_DPRAM_BITS   9
#define MB_FIFO_BITS    4

#define MB_LAST_ADDRESS   ((1 << MB_DPRAM_BITS) - 1)
#define MB_FIFO_BASE      ((1 << MB_DPRAM_BITS) - (1 << MB_FIFO_BITS))
#define MB_RET_AVAIL(r)   ((r) & (1<<MB_FIFO_BITS))
#define MB_PND_MASK       ((1<<MB_FIFO_BITS) - 1)

static volatile bool got_irq;

void VidorIRQ::onInterrupt() {
	got_irq = true;
	getInterruptSource();
	return;
}

static uint32_t interruptMask = (1 << g_APinDescription[IRQ_PIN].ulExtInt);

void VidorIRQ::lock() {
	EIC->INTENCLR.reg = EIC_INTENCLR_EXTINT(interruptMask);
}

void VidorIRQ::unlock() {
	EIC->INTENCLR.reg = EIC_INTENSET_EXTINT(interruptMask);
	getInterruptSource(digitalRead(IRQ_PIN));
}

extern LinkedList<VidorIP*> IPList;

bool VidorIRQ::getInterruptSource(bool force) {
	uint32_t  num;
	uint32_t  code;
	static bool lock = false;

	if (got_irq == false && !force) {
		return false;
	}

	if (lock) {
		return false;
	}
	lock = true;

	VidorMailbox.read(MB_LAST_ADDRESS, &num, 1);
	num &= MB_PND_MASK;

	for (int i = 0; i < num; i++) {
		VidorMailbox.read(MB_FIFO_BASE, &code, 1);
		if (code==0xDEADBEEF) return true;
		VidorIP  *ip;
		for (int j = 0; j < IPList.size(); j++) {
			ip = IPList.get(j);
			if (((ip->info.giid<<24)|(ip->info.chn<<12)) == code) {
				if (ip->cb) {
					ip->cb(NULL, 0, ip);
				}
				break;
			}
		}
	}

	lock = false;
	got_irq = false;
	return true;
}
