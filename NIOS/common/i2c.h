#ifndef I2C_H_
#define I2C_H_

#include <alt_types.h>

void i2cInit(int);
void i2cCmd(void);

alt_u32 i2cEnable(alt_u32 index);
alt_u32 i2cClockSet(alt_u32 index, alt_u32 baud);
alt_u32 i2cDisable(alt_u32 index);
alt_u32 i2cRead(alt_u32 index, alt_u8 address, alt_u8* data, alt_u32 len);
alt_u32 i2cWrite(alt_u32 index, alt_u8 address, alt_u8* data, alt_u32 len);

#endif /* I2C_H_ */
