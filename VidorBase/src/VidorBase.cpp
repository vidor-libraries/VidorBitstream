#include "VidorBase.h"
#include "VidorI2C.h"
#include "VidorSPI.h"
#include "VidorUART.h"

VidorBase VD;

__attribute__ ((used, section(".fpga_bitstream_signature")))
const unsigned char signatures[4096] = {
	//#include "signature.ttf"
	NO_BOOTLOADER,

	0x00, 0x00, 0x08, 0x00,
	0x80, 0xAA, 0x1F, 0x00,
	0x90, 0x22, 0x7f, 0x87, 0x28, 0x51, 0x03, 0x9c, 0x34, 0x6d, 0xe6, 0xd0, 0x23, 0xda, 0x61, 0x85, 0xb6, 0x17, 0x05, 0x4b, 0xc3, 0xda, 0x17, 0xa4, 0x43, 0x6f, 0xd2, 0xd3, 0x92, 0x85, 0x3e, 0x0a,
	0x01, 0x00, 0x00, 0x00,		/* TAG 1 == USER BITSTREAM */
	0x00, 0x00, 0x00, 0x00,		// Don't force

	NO_USER_DATA,
};
__attribute__ ((used, section(".fpga_bitstream")))
const unsigned char bitstream[] = {
	#include "app.ttf"
};

// redefines C-scope pin extended functions (non-weak implementation)

extern "C" {
void pinModeExtended( uint32_t ulPin, uint32_t ulMode ) {
	// TODO: add some checks if ulPin makes sense?
	// ulMode can be INPUT, INPUT_PULLUP or OUTPUT
	VD.pinMode(ulPin - GPIO_NUM_OFFSET, ulMode);
}

void digitalWriteExtended( uint32_t ulPin, uint32_t ulVal ) {
	// No need to check if the pin is alredy in GPIO mode
	VD.digitalWrite(ulPin - GPIO_NUM_OFFSET, ulVal);
}

int digitalReadExtended( uint32_t ulPin ) {
	// No need to check if the pin is alredy in GPIO mode
	return VD.digitalRead(ulPin - GPIO_NUM_OFFSET);
}

void analogWriteExtended( uint32_t ulPin, uint32_t ulVal ) {
	// check if mux for the pin is already in PWM mode and eventually change it
	// set the PWM duty
	return VD.analogWrite(ulPin - GPIO_NUM_OFFSET, ulVal);
}
}

void VidorBase::onInterrupt() {
	// check which module has originated the request
	uint32_t irq = VD.read(IRQ_BASE_ADDRESS);

	// serve the interrupts.
	if (irq | (1 << IRQ_WIRE)) {
		// TODO: handle onReceive() and onRequest()
		// onService only needed in slave mode
		//WireEx.onService();
	}
	if (irq | (1 << IRQ_UART)) {
		SerialEx.onInterrupt() ;
	}
}
