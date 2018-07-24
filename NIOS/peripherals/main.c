/**
 *
 */
#include "platform.h"
#include "gpio.h"

#include "np.h"
void npTest(void)
{
  int i;

  npSet(0xF, 256, 0x58);

  for (i=0; i<256; i++) {
    npLedSet(0, i, i, 255-i, 0, 0);
  }
  npLedShow();
}

#include "spi.h"
#include "mb.h"
void spiTest(alt_u32 idx)
{
  alt_u32 volatile *rpc = (alt_u32*)MB_BASE;
  alt_u8  buf[16];
  alt_u32 len;
  int     i;
  int     err;

  for(i=0; i<16; i++) {
    buf[i] = i+0x30;
  }
  spiEnable(idx);
//  spiModeSet(idx, 20000000, 0, 0);
  spiModeSet(idx, 50000000, 0, 0);

  spiTrx(idx, buf, 16);

  len = 16;
  rpc[0] = MB_DEV_SPI | ((idx & 0x0F)<<20) | 0x04;
  rpc[1] = len;
  memcpy(&rpc[2], "ABCDEFGHIJKLMNOP", len);
  platformCmd();

  for (i=0; i<256; i++) {
    rpc[0] = MB_DEV_SPI | ((idx & 0x0F)<<20) | 0x05;
    rpc[1] = i;
    platformCmd();
    if (rpc[1] != i) {
      err++;
    }
  }

  //spiDisable(idx);
}

#include "uart.h"
void uartTest(void)
{
  alt_u32 idx;
  int c;
  int i;
  alt_u8 buf[32];

  idx = 0;
#if 0
  uartEnable(idx, 0);
  uartSet(idx, 115200, 0);

  for(i=0x20; i<0x80; i++) {
    uartPut(idx, (alt_u8)i);
  }
  uartPut(idx, 0x0D);
  uartPut(idx, 0x0A);

  uartWrite(idx, "Hello\r\n", 6);

  for(;;){
    i = uartAvail(idx);

    if (i>=16) {
      for (c=0; c<i; c++) {
        buf[c] = uartGet(idx);
      }
      uartPut(idx, 0x0D);
      uartPut(idx, 0x0A);
      for (c=0; c<i; c++) {
        uartPut(idx, buf[c]);
      }
      uartPut(idx, 0x0D);
      uartPut(idx, 0x0A);
    }
  }
#endif
#if 1
  alt_u32 volatile *rpc = (alt_u32*)MB_BASE;
  alt_u32 mode;

  //uartEnable(idx, 0);
  mode = 0; // 0-2w, 1-4w, 3-6w
  rpc[0] = MB_DEV_UART | ((idx & 0x0F)<<20) | 0x01;
  rpc[1] = mode;
  platformCmd();

  //uartSet(idx, 115200, 0);
  mode = UART_PARITY_NONE | UART_STOP_BIT_1 | UART_DATA_8;
  rpc[0] = MB_DEV_UART | ((idx & 0x0F)<<20) | 0x02;
  rpc[1] = 115200;
  rpc[2] = mode;
  platformCmd();

  //uartWrite(idx, "Hello\r\n", 6);
  rpc[0] = MB_DEV_UART | ((idx & 0x0F)<<20) | 0x08;
  rpc[1] = 7;
  memcpy(&rpc[2], "Hello\r\n", 7);
  platformCmd();

  for(i=0x20; i<0x80; i++) {
    // uartPut(idx, character);
    rpc[0] = MB_DEV_UART | ((idx & 0x0F)<<20) | 0x07;
    rpc[1] = (alt_u8)i;
    platformCmd();
  }
  //  uartPut(idx, 0x0D);
  rpc[0] = MB_DEV_UART | ((idx & 0x0F)<<20) | 0x07;
  rpc[1] = 0x0D;
  platformCmd();
  //  uartPut(idx, 0x0A);
  rpc[0] = MB_DEV_UART | ((idx & 0x0F)<<20) | 0x07;
  rpc[1] = 0x0A;
  platformCmd();

  for(;;){
    //i = uartAvail(idx);
    rpc[0] = MB_DEV_UART | ((idx & 0x0F)<<20) | 0x06;
    platformCmd();
    i = rpc[1];

    if (i>=16) {
      for (c=0; c<i; c++) {
        //buf[c] = uartGet(idx);
        rpc[0] = MB_DEV_UART | ((idx & 0x0F)<<20) | 0x04;
        platformCmd();
        buf[c] = rpc[1];
      }
      //  uartPut(idx, 0x0D);
      rpc[0] = MB_DEV_UART | ((idx & 0x0F)<<20) | 0x07;
      rpc[1] = 0x0D;
      platformCmd();
      //  uartPut(idx, 0x0A);
      rpc[0] = MB_DEV_UART | ((idx & 0x0F)<<20) | 0x07;
      rpc[1] = 0x0A;
      platformCmd();
      for (c=0; c<i; c++) {
        //uartPut(idx, buf[c]);
        rpc[0] = MB_DEV_UART | ((idx & 0x0F)<<20) | 0x07;
        rpc[1] = buf[c];
        platformCmd();
      }
      //  uartPut(idx, 0x0D);
      rpc[0] = MB_DEV_UART | ((idx & 0x0F)<<20) | 0x07;
      rpc[1] = 0x0D;
      platformCmd();
      //  uartPut(idx, 0x0A);
      rpc[0] = MB_DEV_UART | ((idx & 0x0F)<<20) | 0x07;
      rpc[1] = 0x0A;
      platformCmd();
    }
  }


#endif
}

#include "sf.h"
void sfTest(void)
{
  alt_u32 JedecId;
  alt_u8  UniqueId[8];

  JedecId =  sfJedecId();
  sfUniqueId(UniqueId);
}
/*
#include "sign.h"
void signTest(void)
{
  int ret;
  ret = signChk();
  if (ret) {
    while (1);
  }
}
*/
/**
 *
 */
int main(void)
{
  platformSetup();
#if 0
  npTest();
  //spiTest(2);
  //uartTest();
  //signTest();
  //sfTest();
#endif
  irqPinSet(0, platformCmd);
  intPinInit(1, 0);

  while (1) {
    platformLoop();
  };

  return 0;
}

