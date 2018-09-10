/**
 *
 * @file np.h
 * @brief NeoPixel driver
 *
 */
#ifndef NP_H_
#define NP_H_

#define NEOPIXEL_UID  0xa0894

#define NEOPIXEL_IP_VER   0x0302
#define NEOPIXEL_DRV_VER  0x0406
#define NEOPIXEL_VER      (((NEOPIXEL_IP_VER)<<16)|(NEOPIXEL_DRV_VER))

#define NEOPIXEL_PIN_FNC_OUT PIN_FNC(0x01, PIN_DIR_O)

typedef struct {
  alt_u32   flg;
  alt_u32   num;
  struct {
    alt_u32   ms;
    alt_u16   ofs;
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


void npRpc(void);

#endif /* NP_H_ */

