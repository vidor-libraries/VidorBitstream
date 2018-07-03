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
	0x87, 0x06, 0x04, 0x00,
	0xb0, 0x06, 0x6e, 0xc6, 0x46, 0x88, 0x9e, 0x2e, 0x10, 0xf6, 0x0f, 0xec, 0x09, 0x6d, 0x5b, 0x6f, 0x2a, 0xb3, 0x9e, 0xb3, 0x69, 0x3d, 0x97, 0x9a, 0xb4, 0x3b, 0x23, 0x23, 0x7a, 0x56, 0x45, 0xd1, 
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
