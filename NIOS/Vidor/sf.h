/*
 * sf.h
 *
 *  Created on: Apr 11, 2018
 *      Author: max
 */

#ifndef SF_H_
#define SF_H_

#include <alt_types.h>

int sfCmd(void);

alt_u32 sfDetect(void);
int sfReadId(alt_u8* id);
int sfErase(void);
int sfProgram(alt_u32 adr, alt_u8* data, alt_u32 len);
int sfRead(alt_u32 adr, alt_u8* data, alt_u32 len);

#define SF_WRFLG_ERASE    0x00000001
#define SF_WRFLG_PROGRAM  0x00000002
#define SF_WRFLG_VERIFY   0x00000004
int sfWrite(alt_u8* data, alt_u32 len, alt_u32 flg);

/**
 * Security Register
 */
int sfSRErase(alt_u8 reg);
int sfSRProgram(alt_u8 reg, alt_u8 adr, alt_u8* data, alt_u32 len);
int sfSRLock(alt_u8 reg);
int sfSRRead(alt_u8 reg, alt_u8 adr, alt_u8* data, alt_u32 len);





#endif /* SF_H_ */
