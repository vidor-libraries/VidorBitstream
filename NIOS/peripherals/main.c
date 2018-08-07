/**
 *
 */
#include "platform.h"
#include "gpio.h"


#if 1
#include "config.h"

#include "np.h"
void npTest(void)
{
  int i;
  alt_u32 dev;

  dev = 0;
  npSet(0xF, 256, 0x58);

  for (i=0; i<256; i++) {
    npLedSet(dev, i, i, 255-i, 0, 0);
  }
  for (i=0; i<8; i++) {
    npLedSet(dev, 2+i*8, 0x404040, 0x404040, 0x404040, 0);
  }
  npLedShow();
}
void npSeqTest(void)
{
  alt_u32   nb;
  alt_u32   ret;
  int       i;
  sNpSeq    seq;
  alt_u32   dev;

  dev = 0;
  nb = 4;

  npBufSet(nb, 0, 1, 8);
  ret = npSet(0x0000000F, 256, 0x0058);
  if (ret) {
    //conPrintln("np ERROR configuring neopixel buffer");
  }

  ret = npBufSel(0);
  for (i=0; i<256; i++) {
    npLedSet(dev, i, 255, 0, 0, 0);
  }
  ret = npBufSel(1);
  for (i=0; i<256; i++) {
    npLedSet(dev, i, 0, 255, 0, 0);
  }
  ret = npBufSel(2);
  for (i=0; i<256; i++) {
    npLedSet(dev, i, 0, 0, 255, 0);
  }
  ret = npBufSel(3);
  for (i=0; i<256; i++) {
    npLedSet(dev, i, 255, 255, 255, 0);
  }

  //seq.flg = NP_SEQ_FLG_START | NP_SEQ_FLG_LOOP;
  seq.flg = NP_SEQ_FLG_START | NP_SEQ_FLG_SINGLE;
  //seq.flg = NP_SEQ_FLG_STOP;
  seq.num = 4;
  seq.seq[0].ms = 500; seq.seq[0].nBuf = 0;
  seq.seq[1].ms = 500; seq.seq[1].nBuf = 1;
  seq.seq[2].ms = 500; seq.seq[2].nBuf = 2;
  seq.seq[3].ms = 500; seq.seq[3].nBuf = 3;
  ret = npSeqSet(&seq);
  if (ret) {
    //conPrintln("np ERROR setting sequence");
  }
}
#include "gfx.h"
void npGfxTest(void)
{
  npBufSet(1, 0, 1, 8);
  npBufSel(0);
  npSet(0x0000000F, 256, 0x0058);
#if 1
  npGfxAtt(GFX_GC_ROT90);
  npGfxStrSet(0);
#else
  GFXgc npGc;

  npGc.width  = 8;
  npGc.height = 32;
  npGc.stride = NP_DEV_NUM;
  npGc.bpp    = 32;
  npGc.fmt    = GFX_GC_FMT_XGRB32;
  npGc.fb     = (alt_u8*)NP_MEM_BASE;
  npGc.wp     = 0;
  npGc.flg    = GFX_GC_ROT90;

  gcSet(&npGc);
#endif
  fillRect(0, 0, 32, 8, 0x00000000);
  while(npLedShow()==-1);
/*
  fillRect(0,  0, 2,  4, 0x002F0000);
  fillRect(2,  0, 2,  4, 0x00002F00);
  fillRect(4,  0, 2,  4, 0x0000002F);
  fillRect(6,  0, 2,  4, 0x002F2F2F);

  fillRect(0,  4, 2,  4, 0x002F2F2F);
  fillRect(2,  4, 2,  4, 0x0000002F);
  fillRect(4,  4, 2,  4, 0x00002F00);
  fillRect(6,  4, 2,  4, 0x002F0000);

  fillRect(0,  8, 2, 16, 0x001F0000);
  fillRect(2,  8, 2, 16, 0x00001F00);
  fillRect(4,  8, 2, 16, 0x0000001F);
  fillRect(6,  8, 2, 16, 0x001F1F1F);

  fillRect(0, 24, 2,  4, 0x002F0000);
  fillRect(2, 24, 2,  4, 0x00002F00);
  fillRect(4, 24, 2,  4, 0x0000002F);
  fillRect(6, 24, 2,  4, 0x002F2F2F);

  fillRect(0, 28, 2,  4, 0x002F2F2F);
  fillRect(2, 28, 2,  4, 0x0000002F);
  fillRect(4, 28, 2,  4, 0x00002F00);
  fillRect(6, 28, 2,  4, 0x002F0000);
  while(npLedShow()==-1);

  fillRect(0,  0, 4, 16, 0x000F0000);
  fillRect(4,  0, 4, 16, 0x00000F00);
  fillRect(0, 16, 4, 16, 0x0000000F);
  fillRect(4, 16, 4, 16, 0x000F0F0F);
*/
  fillRect(0, 0, 32, 1, 0x001F0000);
  fillRect(0, 1, 32, 1, 0x00001F00);
  fillRect(0, 2, 32, 1, 0x0000001F);
  fillRect(0, 3, 32, 1, 0x001F1F1F);
  fillRect(0, 4, 32, 1, 0x001F0000);
  fillRect(0, 5, 32, 1, 0x00001F00);
  fillRect(0, 6, 32, 1, 0x0000001F);
  fillRect(0, 7, 32, 1, 0x001F1F1F);

  drawChar( 0, 6, 0x00FFFFFF, 1, 'a');
  drawChar( 8, 6, 0x00FFFFFF, 1, 'b');
  drawChar(16, 6, 0x00FFFFFF, 1, 'c');
  drawChar(24, 6, 0x00FFFFFF, 1, 'd');
  while(npLedShow()==-1);
}
void npWrapTest(void)
{
  int       r;
  alt_u32   cnt;
  alt_u32   adr;
  sNpSeq    seq;

  npBufSet(1, 512, 1, 8);
  npSet(0x0000000F, 256, 0x0058);
#if 1
  npGfxAtt(GFX_GC_ROT90);
  npGfxStrSet(0);
#else
  GFXgc npGc;

  npGc.width  = 8;
  npGc.height = 32;
  npGc.stride = NP_DEV_NUM;
  npGc.bpp    = 32;
  npGc.fmt    = GFX_GC_FMT_XGRB32;
  npGc.fb     = (alt_u8*)NP_MEM_BASE;
  npGc.wp     = 0;
  npGc.flg    = GFX_GC_ROT90;

  gcSet(&npGc);
#endif
  fillRect(0, 0, 32, 8, 0x00000000);
  while(npLedShow()==-1);

  for (r=0; r<8; r++) {
    drawChar(r*8, 6, 0x001F1F1F, 1, 'A'+r);
  }
  while(npLedShow()==-1);
/*
  cnt = 32 + 1;
  adr = 0;

  npWrapSet( 0, cnt, adr); while(npLedShow()==-1);
  npWrapSet( 1, cnt, adr); while(npLedShow()==-1);
  npWrapSet( 2, cnt, adr); while(npLedShow()==-1);
  npWrapSet( 3, cnt, adr); while(npLedShow()==-1);
  npWrapSet( 4, cnt, adr); while(npLedShow()==-1);
  npWrapSet( 5, cnt, adr); while(npLedShow()==-1);
  npWrapSet( 6, cnt, adr); while(npLedShow()==-1);
  npWrapSet( 7, cnt, adr); while(npLedShow()==-1);
  npWrapSet(16, cnt, adr); while(npLedShow()==-1);
  npWrapSet(32, cnt, adr); while(npLedShow()==-1);

  cnt = (64 - 32); npWrapSet(32, cnt, adr); while(npLedShow()==-1);
  cnt = (64 - 40); npWrapSet(40, cnt, adr); while(npLedShow()==-1);
  cnt = (64 - 48); npWrapSet(48, cnt, adr); while(npLedShow()==-1);
  cnt = (64 - 56); npWrapSet(56, cnt, adr); while(npLedShow()==-1);
  cnt = (64 - 57); npWrapSet(57, cnt, adr); while(npLedShow()==-1);
  cnt = (64 - 58); npWrapSet(58, cnt, adr); while(npLedShow()==-1);
  cnt = (64 - 59); npWrapSet(59, cnt, adr); while(npLedShow()==-1);
  cnt = (64 - 60); npWrapSet(60, cnt, adr); while(npLedShow()==-1);
  cnt = (64 - 61); npWrapSet(61, cnt, adr); while(npLedShow()==-1);
  cnt = (64 - 62); npWrapSet(62, cnt, adr); while(npLedShow()==-1);
  cnt = (64 - 63); npWrapSet(63, cnt, adr); while(npLedShow()==-1);
*/


//  npBufLoop(NP_SEQ_FLG_START | NP_SEQ_FLG_BUF_LOOP, 0, 500);
  npBufLoop(NP_SEQ_FLG_START | NP_SEQ_FLG_BUF_LOOP | NP_SEQ_FLG_INV_LOOP, 0, 500);

/*
  seq.flg = NP_SEQ_FLG_START | NP_SEQ_FLG_LOOP;
  //seq.flg = NP_SEQ_FLG_START | NP_SEQ_FLG_SINGLE;
  //seq.flg = NP_SEQ_FLG_STOP;
  seq.num = 64;
  for(r=0; r<64; r++) {
    seq.seq[r].ms = 500;
    seq.seq[r].nBuf = 0;
    seq.seq[r].sAdr = r;
    seq.seq[r].wCnt = 64 - r;
    seq.seq[r].wAdr = 0;
  }
  npSeqSet(&seq);
*/
}


#ifdef TEST_SPI

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
#endif /* TEST_SPI */

#ifdef TEST_UART

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
#endif /* TEST_UART */

#endif

/**
 *
 */
int main(void)
{
  platformSetup();

#if 1
  //npTest();
  //npSeqTest();
  npGfxTest();
  npWrapTest();
  //spiTest(2);
  //uartTest();
#endif

  irqPinSet(0, platformCmd);
  intPinInit(1, 0);

  while (1) {
    platformLoop();
  };

  return 0;
}

