/**
 *
 * @file np.h
 * @brief NeoPixel driver
 *
 */
#ifndef NP_H_
#define NP_H_

void npInit(int);
void npCmd(void);

alt_u32 npTmgSet(alt_u32 frq, alt_u32 trst, alt_u32 t0h, alt_u32 t1h, alt_u32 ttot);
alt_u32 npSet(alt_u32 mask, alt_u16 numLEDs, alt_u32 type);
alt_u32 npLedSet(alt_u32 idx, alt_u16 LED, alt_u8 r, alt_u8 g, alt_u8 b, alt_u8 w);
alt_u32 npBrgSet(alt_u32 idx, alt_u8 brg);
alt_u32 npLedShow(void);

#endif /* NP_H_ */

