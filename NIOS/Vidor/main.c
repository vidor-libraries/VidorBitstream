/* 
 * "Small Hello World" example. 
 * 
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example 
 * designs. It requires a STDOUT  device in your system's hardware. 
 *
 * The purpose of this example is to demonstrate the smallest possible Hello 
 * World application, using the Nios II HAL library.  The memory footprint
 * of this hosted application is ~332 bytes by default using the standard 
 * reference design.  For a more fully featured Hello World application
 * example, see the example titled "Hello World".
 *
 * The memory footprint of this example has been reduced by making the
 * following changes to the normal "Hello World" example.
 * Check in the Nios II Software Developers Manual for a more complete 
 * description.
 * 
 * In the SW Application project (small_hello_world):
 *
 *  - In the C/C++ Build page
 * 
 *    - Set the Optimization Level to -Os
 * 
 * In System Library project (small_hello_world_syslib):
 *  - In the C/C++ Build page
 * 
 *    - Set the Optimization Level to -Os
 * 
 *    - Define the preprocessor option ALT_NO_INSTRUCTION_EMULATION 
 *      This removes software exception handling, which means that you cannot 
 *      run code compiled for Nios II cpu with a hardware multiplier on a core 
 *      without a the multiply unit. Check the Nios II Software Developers 
 *      Manual for more details.
 *
 *  - In the System Library page:
 *    - Set Periodic system timer and Timestamp timer to none
 *      This prevents the automatic inclusion of the timer driver.
 *
 *    - Set Max file descriptors to 4
 *      This reduces the size of the file handle pool.
 *
 *    - Check Main function does not exit
 *    - Uncheck Clean exit (flush buffers)
 *      This removes the unneeded call to exit when main returns, since it
 *      won't.
 *
 *    - Check Don't use C++
 *      This builds without the C++ support code.
 *
 *    - Check Small C library
 *      This uses a reduced functionality C library, which lacks  
 *      support for buffering, file IO, floating point and getch(), etc. 
 *      Check the Nios II Software Developers Manual for a complete list.
 *
 *    - Check Reduced device drivers
 *      This uses reduced functionality drivers if they're available. For the
 *      standard design this means you get polled UART and JTAG UART drivers,
 *      no support for the LCD driver and you lose the ability to program 
 *      CFI compliant flash devices.
 *
 *    - Check Access device drivers directly
 *      This bypasses the device file system to access device drivers directly.
 *      This eliminates the space required for the device file system services.
 *      It also provides a HAL version of libc services that access the drivers
 *      directly, further reducing space. Only a limited number of libc
 *      functions are available in this configuration.
 *
 *    - Use ALT versions of stdio routines:
 *
 *           Function                  Description
 *        ===============  =====================================
 *        alt_printf       Only supports %s, %x, and %c ( < 1 Kbyte)
 *        alt_putstr       Smaller overhead than puts with direct drivers
 *                         Note this function doesn't add a newline.
 *        alt_putchar      Smaller overhead than putchar with direct drivers
 *        alt_getchar      Smaller overhead than getchar with direct drivers
 *
 */

#include <stdio.h>
#include <string.h>
#include <sys/alt_stdio.h>
#include <system.h>

#include "i2c.h"
#include "gpio.h"
#include "mb.h"
#include "sf.h"
#include "gfx.h"
#include "spi.h"
#include "uart.h"

#define DEBUG_PRINT_ENABLED 0
#if DEBUG_PRINT_ENABLED
#define DGB_PRINTF printf
#else
#define DGB_PRINTF(format, args...) ((void)0)
#endif

void rxCmd(alt_u32 cmd);

/**
 * TODO togliere, solo per debug
 */
void loop(void)
{
	volatile alt_u32 cmd;
	int ret;
	cmd = *(volatile alt_u32*)DPRAM_BASE;
	if (cmd) {
		switch (cmd & MB_DEV_MSK){
		case MB_DEV_SF  : ret = sfCmd(); break;
		case MB_DEV_GPIO: ret = gpioCmd(); break;
		case MB_DEV_GFX : ret = gfxCmd(); break;
		case MB_DEV_I2C : ret = i2cCmd(); break;
		case MB_DEV_SPI : ret = spiCmd(); break;
		case MB_DEV_UART: ret = uartCmd(); break;
		}
		*(volatile alt_u32*)DPRAM_BASE = 0;
	}
}

#if 0
void i2cTest(void)
{
	int index = 0;
	alt_u32 volatile *rpc = (alt_u32*)DPRAM_BASE;

	// enable I2C index 0 - CSI_I2C_BASE
	rpc[0] = MB_DEV_I2C | ((index & 0x0F)<<20) | 0x01;
	loop();

	// start transmission
	rpc[1] = 0x36;
	rpc[2] = 0x00;
	rpc[0] = MB_DEV_I2C | ((index & 0x0F)<<20) | 0x04;
	loop();

	// write
	rpc[1] = 0x01;
	rpc[0] = MB_DEV_I2C | ((index & 0x0F)<<20) | 0x08;
	loop();

	rpc[1] = 0x03;
	rpc[0] = MB_DEV_I2C | ((index & 0x0F)<<20) | 0x08;
	loop();

	rpc[1] = 0x01;
	rpc[0] = MB_DEV_I2C | ((index & 0x0F)<<20) | 0x08;
	loop();

	// stop
	rpc[0] = MB_DEV_I2C | ((index & 0x0F)<<20) | 0x07;
	loop();
}
#endif
void gpioTest(void)
{
	alt_u32 volatile *rpc = (alt_u32*)DPRAM_BASE;
	alt_u32 pin;
	alt_u32 mode;
	alt_u32 val;
#if 0
	/* set mode */
	pin  = 32;
	mode = 2;
	rpc[1] = pin;
	rpc[2] = mode;
	rpc[0] = MB_DEV_GPIO | 0x01;
	loop();

	/* write */
	val = 0;
	rpc[1] = pin;
	rpc[2] = val;
	rpc[0] = MB_DEV_GPIO | 0x02;
	loop();

	/* write */
	val = 1;
	rpc[1] = pin;
	rpc[2] = val;
	rpc[0] = MB_DEV_GPIO | 0x02;
	loop();

	/* read */
	rpc[1] = pin;
	rpc[0] = MB_DEV_GPIO | 0x03;
	loop();
#endif

	/* pwm */
	pin  = 32;
	mode = 3;
	rpc[1] = pin;
	rpc[2] = mode;
	rpc[0] = MB_DEV_GPIO | 0x01;
	loop();

	rpc[1] = 1000;//prescaler
	rpc[2] = 120;//period
	rpc[0] = MB_DEV_GPIO | 0x04;
	loop();

	rpc[1] = pin;
	rpc[2] = 60;
	rpc[3] = 40;
	rpc[0] = MB_DEV_GPIO | 0x05;
	loop();

}

/**
 *
 */
int main()
{
	int ret;

	// update serial flash with FPGA binary
	//ret = sfWrite(0x00800000, 181344, SF_WRFLG_ERASE | SF_WRFLG_PROGRAM | SF_WRFLG_VERIFY);
	//ret = sfWrite(0x00800000, 181344, SF_WRFLG_VERIFY);
/*
	alt_u32 sf_id;
	alt_u8  sf_sn[8];

	sf_id = sfDetect();
	sfReadId(sf_sn);

	// Security Register test
	alt_u8 data[256];

	sfSRRead(0, 0, data, 256);
	strcpy(data, "111-Security Register write test-111");
	sfSRProgram(0, 0, data, 256);

	sfSRRead(1, 0, data, 256);
	strcpy(data, "222-Security Register write test-222");
	sfSRProgram(1, 0, data, 256);

	sfSRRead(2, 0, data, 256);
	strcpy(data, "333-Security Register write test-333");
	sfSRProgram(2, 0, data, 256);

	sfSRRead(0, 0, data, 256);
	sfSRRead(1, 0, data, 256);
	sfSRRead(2, 0, data, 256);
*/
/*
	sfSRErase(alt_u8 reg);

	sfSRLock(alt_u8 reg);
*/

//	i2cTest();
	gpioTest();

//	mbInit(rxCmd);

	//gfxInit();


//	i2c_init(CSI_I2C_BASE);

	/* Event loop never exits. */
//	alt_u32 cmd;
	while (1) {
		/*
		cmd = mbPoll();
		if (cmd) {
			switch (cmd & MB_DEV_MSK){
			case MB_DEV_SF  : ret = sfCmd(); break;
			case MB_DEV_GPIO: ret = gpioCmd(); break;
			case MB_DEV_GFX : ret = gfxCmd(); break;
			case MB_DEV_I2C : ret = i2cCmd(); break;
			case MB_DEV_SPI: break;
			case MB_DEV_UART: break;
			}
			mbDone();
		}
		*/
	};

	return 0;
}

/**
 *
 */
void rxCmd(alt_u32 cmd)
{
	int ret;

	switch (cmd & MB_DEV_MSK){
	case MB_DEV_SF  : ret = sfCmd(); break;
	case MB_DEV_GPIO: ret = gpioCmd(); break;
	case MB_DEV_GFX : ret = gfxCmd(); break;
	case MB_DEV_I2C : ret = i2cCmd(); break;
	case MB_DEV_SPI : ret = spiCmd(); break;
	case MB_DEV_UART: ret = uartCmd(); break;
	}
	mbDone();
	//mbTxSend(0x00000001, ret);
}
