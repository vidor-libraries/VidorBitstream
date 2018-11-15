#include "VidorIRQ.h"
#include "VidorMailbox.h"

#define MB_DPRAM_BITS   9
#define MB_FIFO_BITS    4

#define MB_LAST_ADDRESS   ((1 << MB_DPRAM_BITS) - 1)
#define MB_FIFO_BASE      ((1 << MB_DPRAM_BITS) - (1 << MB_FIFO_BITS))
#define MB_RET_AVAIL(r)   ((r) & (1<<MB_FIFO_BITS))
#define MB_PND_MASK       ((1<<MB_FIFO_BITS) - 1)

static volatile bool got_irq;

void VidorIRQ::onInterrupt() {
	SerialUSB.println("got irq");
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


bool VidorIRQ::getInterruptSource(bool force) {
	uint32_t  num;
	uint32_t  code;
	int       i;

	if (got_irq == false && !force) {
		return false;
	}

	VidorMailbox.read(MB_LAST_ADDRESS, &num, 1);
	SerialUSB.println("ISR last " + String(num, HEX));
	num &= MB_PND_MASK;

	for (i=0; i < num; i++) {
		VidorMailbox.read(MB_FIFO_BASE, &code, 1);
		SerialUSB.println("ISR code " + String(code, HEX));
	}

	got_irq = false;
	return true;
}
