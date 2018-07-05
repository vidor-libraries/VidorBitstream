#include "VidorBase.h"
#include "VidorI2C.h"
#include "VidorSPI.h"
#include "VidorUART.h"

VidorBase VD;

#if 1
__attribute__ ((used, section(".fpga_bitstream_signature")))
const unsigned char signatures[4096] = {
	//#include "signature.ttf"
	NO_BOOTLOADER,

	0x00, 0x00, 0x08, 0x00,
	0xA9, 0x6F, 0x1F, 0x00,
	0x6f, 0x90, 0x25, 0x62, 0xb4, 0xec, 0xae, 0x0a, 0xa7, 0x28, 0x22, 0xc6, 0x91, 0xbf, 0x61, 0x65, 0xef, 0x0d, 0x9d, 0x7c, 0x04, 0x19, 0x82, 0x3b, 0x0a, 0xf1, 0x4e, 0x00, 0x00, 0xff, 0xf0, 0x0f,
	0x01, 0x00, 0x00, 0x00,		/* TAG 1 == USER BITSTREAM */
	0x00, 0x00, 0x00, 0x00,		// Don't force

	NO_USER_DATA,
};
__attribute__ ((used, section(".fpga_bitstream")))
const unsigned char bitstream[] = {
	#include "app.ttf"
};
#endif

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
	pinModeExtended(ulPin, 2);
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
