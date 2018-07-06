#include "VidorBase.h"

#if FPGA_I2C_INTERFACES_COUNT > 0
#include "VidorI2C.h"
#endif

#if FPGA_SPI_INTERFACES_COUNT > 0
#include "VidorSPI.h"
#endif

#if FPGA_UART_INTERFACES_COUNT > 0
#include "VidorUART.h"
#endif

#if FPGA_ENCODERS_COUNT > 0
#include "VidorEncoder.h"
#endif

#if FPGA_NEOPIXEL_COUNT > 0
#include "VidorNeopixel.h"
#endif

#if FPGA_CAMERA_COUNT > 0
#include "VidorCamera.h"
#endif

#if FPGA_GFX_COUNT > 0
#include "Vidor_GFX.h"
#endif

#if FPGA_QR_COUNT > 0
#include "VidorQR.h"
#endif

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

void VidorBase::onInterrupt() {
	// TODO: do be implemented
}