#ifndef __COMMON_VIDOR_H__
#define __COMMON_VIDOR_H__

#define MB_BASE         0x00000000
#define MB_DEV_SF       0x01000000
#define MB_DEV_GPIO     0x02000000
#define MB_DEV_GFX      0x03000000
#define MB_DEV_I2C      0x04000000
#define MB_DEV_SPI      0x05000000
#define MB_DEV_UART     0x06000000
#define MB_DEV_QR       0x07000000
#define MB_DEV_SDRAM    0x08000000
#define MB_DEV_NP       0x09000000
#define MB_DEV_ENC      0x0A000000
#define MB_DEV_REG      0x0B000000

static char* configToString(uint32_t cod)
{
	switch(cod){
		case MB_DEV_SF: 	return "MB_DEV_SF";
		case MB_DEV_GPIO: 	return "MB_DEV_GPIO";
		case MB_DEV_GFX: 	return "MB_DEV_GFX";
		case MB_DEV_I2C: 	return "MB_DEV_I2C";
		case MB_DEV_SPI: 	return "MB_DEV_SPI";
		case MB_DEV_UART: 	return "MB_DEV_UART";
		case MB_DEV_QR: 	return "MB_DEV_QR";
		case MB_DEV_SDRAM: 	return "MB_DEV_SDRAM";
		case MB_DEV_ENC: 	return "MB_DEV_ENC";
		case MB_DEV_NP: 	return "MB_DEV_NP";
		case MB_DEV_REG: 	return "MB_DEV_REG";
	}
}

typedef struct {
  int num;
  struct {
    uint32_t cod;
    uint32_t num;
  } dev[32];
} sFpgaCfg, *psFpgaCfg;

#endif
