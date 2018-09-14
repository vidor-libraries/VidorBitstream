/*
 * platform.c
 *
 *  Created on: May 9, 2018
 *      Author: max
 */
#include <stdio.h>

#include "config.h"
#include "mb.h"
#include "irq.h"

#if defined(I2C_MODULE) && (I2C_MODULE == 1)
  #include "i2c.h"
  #define I2C_DEV {i2cInit, i2cCmd, NULL, MB_DEV_I2C, I2C_DEV_NUM, I2C_DID, I2C_VER, I2C_SUB, I2C_CHN},
#else
  #define I2C_DEV
#endif /* defined(I2C_MODULE) && (I2C_MODULE == 1) */

#if defined(GPIO_MODULE) && (GPIO_MODULE == 1)
  #include "gpio.h"
  #define GPIO_DEV {NULL, gpioCmd, NULL, MB_DEV_GPIO, 1, GPIO_DID, GPIO_VER, GPIO_SUB, GPIO_CHN},
#else
  #define GPIO_DEV
#endif /* defined(GPIO_MODULE) && (GPIO_MODULE == 1) */

#if defined(PWM_MODULE) && (PWM_MODULE == 1)
  #include "pwm.h"
  #define PWM_DEV {NULL, pwmCmd, NULL, MB_DEV_PWM, 1, PWM_DID, PWM_VER, PWM_SUB, PWM_CHN},
#else
  #define PWM_DEV
#endif /* defined(PWM_MODULE) && (PWM_MODULE == 1) */

#if defined(SF_MODULE) && (SF_MODULE == 1)
  #include "sf.h"
  #define SF_DEV {sfInit, sfCmd, NULL, MB_DEV_SF, 1, SF_DID, SF_VER, SF_SUB, SF_CHN},
#else
  #define SF_DEV
#endif /* defined(GFX_MODULE) && (GFX_MODULE == 1) */

#if defined(GFX_MODULE) && (GFX_MODULE == 1)
  #include "gfx.h"
  #define GFX_DEV {NULL, gfxCmd, NULL, MB_DEV_GFX, 1, GFX_DID, GFX_VER, GFX_SUB, GFX_CHN},
#else
  #define GFX_DEV
#endif /* defined(GFX_MODULE) && (GFX_MODULE == 1) */

#if defined(SPI_MODULE) && (SPI_MODULE == 1)
  #include "spi.h"
  #define SPI_DEV  {spiInit, spiCmd, NULL, MB_DEV_SPI, SPI_DEV_NUM, SPI_DID, SPI_VER, SPI_SUB, SPI_CHN},
#else
  #define SPI_DEV
#endif /* defined(SPI_MODULE) && (SPI_MODULE == 1) */

#if defined(UART_MODULE) && (UART_MODULE == 1)
  #include "uart.h"
  #define UART_DEV {uartInit, uartCmd, NULL, MB_DEV_UART, UART_DEV_NUM, UART_DID, UART_VER, UART_SUB, UART_CHN},
#else
  #define UART_DEV
#endif /* defined(UART_MODULE) && (UART_MODULE == 1) */

#if defined(QR_MODULE) && (QR_MODULE == 1)
  #include "qr.h"
  #define QR_DEV  {qrInit, qrCmd, qrLoop, MB_DEV_QR, 1, QR_DID, QR_VER, QR_SUB, QR_CHN},
#else
  #define QR_DEV
#endif /* defined(QR_MODULE) && (QR_MODULE == 1) */

#if defined(SDRAM_MODULE) && (SDRAM_MODULE == 1)
  #include "sdram.h"
  #define SDRAM_DEV {sdramInit, sdramCmd, NULL, MB_DEV_SDRAM, 1, SDRAM_DID, SDRAM_VER, SDRAM_SUB, SDRAM_CHN},
#else
  #define SDRAM_DEV
#endif /* defined(SDRAM_MODULE) && (SDRAM_MODULE == 1) */

#if defined(NP_MODULE) && (NP_MODULE == 1)
  #include "np.h"
  #define NP_DEV {npInit, npCmd, NULL, MB_DEV_NP, NP_DEV_NUM, NP_DID, NP_VER, NP_SUB, NP_CHN},
#else
  #define NP_DEV
#endif /* defined(NP_MODULE) && (NP_MODULE == 1) */

#if defined(ENC_MODULE) && (ENC_MODULE == 1)
  #include "enc.h"
  #define ENC_DEV {encInit, encCmd, NULL, MB_DEV_ENC, ENC_DEV_NUM, ENC_DID, ENC_VER, ENC_SUB, ENC_CHN},
#else
  #define ENC_DEV
#endif /* defined(ENC_MODULE) && (ENC_MODULE == 1) */

#if defined(REG_MODULE) && (REG_MODULE == 1)
  #include "reg.h"
  #define REG_DEV {NULL, regCmd, NULL, MB_DEV_REG, 0, REG_DID, REG_VER, REG_SUB, REG_CHN},
#else
  #define REG_DEV
#endif /* defined(REG_MODULE) && (REG_MODULE == 1) */

#define SEC_RAM  __attribute__((__section__(".rwdata")))

/**
 */
typedef struct {
  void(*setup)(int);
  void(*cmd)(void);
  void(*loop)(void);
  alt_u32 dev_cod;
  alt_u32 sub_devs;
  alt_u32 did;
  alt_u16 ver;
  alt_u8  sub;
  alt_u8  chn;
}sDevHnd, *psDevHnd;

/**
 *
 */
void fpgaCmd(void);

/**
 *
 */
sDevHnd const devHnd[] = {
  {NULL, fpgaCmd, NULL, 0, 0},
  SF_DEV
  GPIO_DEV
  GFX_DEV
  I2C_DEV
  SPI_DEV
  UART_DEV
  QR_DEV
  SDRAM_DEV
  NP_DEV
  ENC_DEV
  REG_DEV
//  PWM_DEV  TODO ora è in gpio
};

/**
 */
void platformSetup(void)
{
  int i;

  configInit();

  for (i=0; i<sizeof(devHnd)/sizeof(sDevHnd); i++) {
    if (devHnd[i].setup) {
      devHnd[i].setup(devHnd[i].sub_devs);
    }
  }
}

/**
 */
void SEC_RAM platformCmd(void)
{
  volatile alt_u32 cmd;

  cmd = *(volatile alt_u32*)MB_BASE;
  if (cmd) {
    int dev;
    int i;
    dev = cmd & MB_DEV_MSK;
    for (i=0; i<sizeof(devHnd)/sizeof(sDevHnd); i++) {
      if (devHnd[i].dev_cod == dev) {
          if (devHnd[i].cmd) {
            devHnd[i].cmd();
            break;
          }
      }
    }
    *(volatile alt_u32*)MB_BASE = 0;
    intPinSet(1, 1);
    intPinSet(1, 0);
  }
}

/**
 */
void SEC_RAM platformLoop(void)
{
  int i;

  for (i=0; i<sizeof(devHnd)/sizeof(sDevHnd); i++) {
    if (devHnd[i].loop) {
      devHnd[i].loop();
    }
  }
}

/**
 *
 */
void fpgaCmd(void)
{
  alt_u32 volatile *rpc = (alt_u32*)MB_BASE;
  alt_u32 ret;
  int i;

  ret = -1;
  switch(MB_CMD(rpc[0])){
  case 1:
    /* get version */
    ret = FPGA_VERSION;
    break;
  case 2:
    /* get IP list and number of instances for each device */
    rpc[1] = sizeof(devHnd)/sizeof(sDevHnd) -1;
    for (i=1; i<sizeof(devHnd)/sizeof(sDevHnd); i++) {
      rpc[i*2+0] = devHnd[i].dev_cod;
      rpc[i*2+1] = devHnd[i].sub_devs;
    }
    ret = rpc[1];
    break;
#if defined(FPGA_CMD_JMP) && (FPGA_CMD_JMP == 1)
  case 3:
    rpc[1] = 0x008E0000;
    rpc[0] = 0;

    irqPinSet(0, NULL);

    /* start application */
    __asm__ volatile (
        "movhi r12, 0x008E\n"
        "ori r12, r12, 0x0000\n"
        "jmp r12");
    break;
#endif

  case 20:    // TODO modificare in 2
    /* get IP list, number of instances and channels for each device */
    rpc[1] = sizeof(devHnd)/sizeof(sDevHnd) - 1;
    for (i=1; i<sizeof(devHnd)/sizeof(sDevHnd); i++) {
      rpc[i*2+0] = devHnd[i].did;
      rpc[i*2+1] = (devHnd[i].ver << 16) | (devHnd[i].sub << 8) | devHnd[i].chn ;
    }
    ret = rpc[1];
    break;

  }
  rpc[1] = ret;
}
