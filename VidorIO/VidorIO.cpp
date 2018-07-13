#include "VidorIO.h"


// redefines C-scope pin extended functions (non-weak implementation)

int VidorIO::period = -1;

extern "C" {
void pinModeExtended( uint32_t ulPin, uint32_t ulMode ) {
	// TODO: add some checks if ulPin makes sense?
	// ulMode can be INPUT, INPUT_PULLUP or OUTPUT
	VidorIO::pinMode(ulPin - GPIO_NUM_OFFSET, ulMode);
}

void digitalWriteExtended( uint32_t ulPin, uint32_t ulVal ) {
	// No need to check if the pin is alredy in GPIO mode
	VidorIO::digitalWrite(ulPin - GPIO_NUM_OFFSET, ulVal);
}

int digitalReadExtended( uint32_t ulPin ) {
	// No need to check if the pin is alredy in GPIO mode
	return VidorIO::digitalRead(ulPin - GPIO_NUM_OFFSET);
}

void analogWriteExtended( uint32_t ulPin, uint32_t ulVal ) {
	// check if mux for the pin is already in PWM mode and eventually change it
	pinModeExtended(ulPin, 3);
	// set the PWM duty
	return VidorIO::analogWrite(ulPin - GPIO_NUM_OFFSET, ulVal);
}
}