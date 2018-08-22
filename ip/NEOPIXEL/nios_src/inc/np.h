/**
 *
 * @file np.h
 * @brief NeoPixel driver
 *
 */
#ifndef NP_H_
#define NP_H_

typedef struct {
  alt_u32   flg;
  alt_u32   num;
  struct {
    alt_u32   ms;
    alt_u16   nBuf;
    alt_u16   sAdr;
    alt_u16   wCnt;
    alt_u16   wAdr;
  }         seq[64];
}sNpSeq, *psNpSeq;

#define NP_SEQ_FLG_START      0x00000001
#define NP_SEQ_FLG_STOP       0x00000002
#define NP_SEQ_FLG_SINGLE     0x00000004
#define NP_SEQ_FLG_LOOP       0x00000008
#define NP_SEQ_FLG_BUF_LOOP   0x00000010
#define NP_SEQ_FLG_INV_LOOP   0x00000020

void npInit(int);
void npCmd(void);

alt_u32 npTmgSet(alt_u32 frq, alt_u32 trst, alt_u32 t0h, alt_u32 t1h, alt_u32 ttot);
alt_u32 npSet(alt_u32 mask, alt_u16 numLEDs, alt_u32 type);
alt_u32 npLedSet(alt_u32 idx, alt_u16 LED, alt_u8 r, alt_u8 g, alt_u8 b, alt_u8 w);
alt_u32 npBrgSet(alt_u32 idx, alt_u8 brg);
alt_u32 npLedShow(void);

alt_u32 npBufSet(alt_u32 num, alt_u32 len, alt_u32 zzf, alt_u32 zzl);
alt_u32 npBufSel(alt_u32 idx);
alt_u32 npWrapSet(alt_u32 sAdr, alt_u32 wCnt, alt_u32 wAdr);
alt_u32 npSeqSet(psNpSeq pSeq);
alt_u32 npBufLoop(alt_u32 flg, alt_u32 buf, alt_u32 ms);
alt_u32 npGfxAtt(alt_u32 flg);
alt_u32 npGfxDet(void);
alt_u32 npGfxStrSet(alt_u32 idx);
alt_u32 npGfxWp(alt_u32 x, alt_u32 y, alt_u32 color);

#endif /* NP_H_ */

