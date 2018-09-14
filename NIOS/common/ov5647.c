/*
* ov5647.c
*
*  Created on: Apr 11, 2018
*      Author: max
*/


#include "stdio.h"
#include "system.h"
#include "i2c.h"

#define DEBUG_PRINT_ENABLED 0
#if DEBUG_PRINT_ENABLED
#define DGB_PRINTF printf
#else
#define DGB_PRINTF(format, args...) ((void)0)
#endif

#define ARRAY_SIZE(a)   (sizeof(a) / sizeof(struct regval_list))

#define OV5647_I2C          0
#define OV5647_ADDR         0x36

#define OV5647_SW_RESET       0x0103
#define OV5647_REG_CHIPID_H   0x300A
#define OV5647_REG_CHIPID_L   0x300B

struct regval_list {
  unsigned short  addr;
  unsigned char   data;
};


static int __sensor_init(void);
static int set_sw_standby(int standby);
static int ov5647_write_array(struct regval_list *regs, int array_size);
static int ov5647_set_virtual_channel(int channel);
static int ov5647_wr(alt_u8 address, alt_u16 reg, alt_u8 data);
static int ov5647_rd(alt_u8 address, alt_u16 reg, alt_u8* data, alt_u8 len);



static struct regval_list ov5647_640x480[] = {
  {0x0100, 0x00},
  {0x0103, 0x01},
  {0x3034, 0x08},
  {0x3035, 0x41},
  {0x3036, 0x46},
  {0x303c, 0x11},
  {0x3106, 0xf5},
  {0x3821, 0x07},
  {0x3820, 0x41},
  {0x3827, 0xec},
  {0x370c, 0x0f},
  {0x3612, 0x59},
  {0x3618, 0x00},
  {0x5000, 0x06},
  {0x5001, 0x00},
  {0x5002, 0x40},
  {0x5003, 0x08},
  {0x5a00, 0x08},
  {0x3000, 0x00},
  {0x3001, 0x00},
  {0x3002, 0x00},
  {0x3016, 0x08},
  {0x3017, 0xe0},
  {0x3018, 0x44},
  {0x301c, 0xf8},
  {0x301d, 0xf0},
  {0x3a18, 0x00},
  {0x3a19, 0xf8},
  {0x3c01, 0x80},
  {0x3b07, 0x0c},
  {0x380c, 0x07},
  {0x380d, 0x68},
  {0x380e, 0x03},
  {0x380f, 0xd8},
  {0x3814, 0x31},
  {0x3815, 0x31},
  {0x3708, 0x64},
  {0x3709, 0x52},
  {0x3808, 0x02},
  {0x3809, 0x80},
  {0x380a, 0x01},
  {0x380b, 0xe0},
  {0x3800, 0x00},
  {0x3801, 0x00},
  {0x3802, 0x00},
  {0x3803, 0x00},
  {0x3804, 0x0a},
  {0x3805, 0x3f},
  {0x3806, 0x07},
  {0x3807, 0xa1},
  {0x3811, 0x08},
  {0x3813, 0x02},
  {0x3630, 0x2e},
  {0x3632, 0xe2},
  {0x3633, 0x23},
  {0x3634, 0x44},
  {0x3636, 0x06},
  {0x3620, 0x64},
  {0x3621, 0xe0},
  {0x3600, 0x37},
  {0x3704, 0xa0},
  {0x3703, 0x5a},
  {0x3715, 0x78},
  {0x3717, 0x01},
  {0x3731, 0x02},
  {0x370b, 0x60},
  {0x3705, 0x1a},
  {0x3f05, 0x02},
  {0x3f06, 0x10},
  {0x3f01, 0x0a},
  {0x3a08, 0x01},
  {0x3a09, 0x27},
  {0x3a0a, 0x00},
  {0x3a0b, 0xf6},
  {0x3a0d, 0x04},
  {0x3a0e, 0x03},
  {0x3a0f, 0x58},
  {0x3a10, 0x50},
  {0x3a1b, 0x58},
  {0x3a1e, 0x50},
  {0x3a11, 0x60},
  {0x3a1f, 0x28},
  {0x4001, 0x02},
  {0x4004, 0x02},
  {0x4000, 0x09},
  {0x4837, 0x24},
  {0x4050, 0x6e},
  {0x4051, 0x8f},
  {0x503d, 0x00},
  {0x0100, 0x01},
};


/**
 *
 */
int ov5647Detect(void)
{
  unsigned char read;
  int ret;

  ret = ov5647_wr(OV5647_ADDR, OV5647_SW_RESET, 0x01);
  if (ret < 0) {
    return ret;
  }
  ret = ov5647_rd(OV5647_ADDR, OV5647_REG_CHIPID_H, &read, 1);
  if (ret < 0) {
    return ret;
  }
  if (read != 0x56) {
    DGB_PRINTF("ID High expected 0x56 got %x", read);
    return -1;
  }

  ret = ov5647_rd(OV5647_ADDR, OV5647_REG_CHIPID_L, &read, 1);
  if (ret < 0) {
    return ret;
  }
  if (read != 0x47) {
    DGB_PRINTF("ID Low expected 0x47 got %x", read);
    return -1;
  }

  return ov5647_wr(OV5647_ADDR, OV5647_SW_RESET, 0x00);
}

/**
 */
int ov5647SensorPower(int on)
{
  int ret = 0;

  if (on) {
    ret = __sensor_init();
    if (ret < 0) {
      DGB_PRINTF("Camera not available, check Power\n");
      goto out;
    }
  } else if (!on) {
    DGB_PRINTF(&client->dev, "OV5647 power off\n");

//		ret = ov5647_write_array( sensor_oe_disable_regs,
//				ARRAY_SIZE(sensor_oe_disable_regs));
    if (ret < 0) {
      DGB_PRINTF( "disable oe failed\n");
    }

    ret = set_sw_standby(1);
    if (ret < 0) {
      DGB_PRINTF("soft stby failed\n");
    }
  }

out:
  return ret;
}

/**
 *
 */
int ov5647StreamOn(void)
{
  int ret;

  ret = ov5647_wr(OV5647_ADDR, 0x4202, 0x00);
  if (ret < 0) {
    return ret;
  }
  return ov5647_wr(OV5647_ADDR, 0x300D, 0x00);
}

/**
 *
 */
int ov5647StreamOff(void)
{
  int ret;

  ret = ov5647_wr(OV5647_ADDR, 0x4202, 0x0f);
  if (ret < 0) {
    return ret;
  }
  return ov5647_wr(OV5647_ADDR, 0x300D, 0x01);
}

/**
 *
 */
static int __sensor_init(void)
{
  int ret;
  unsigned char resetval, rdval;

  ret = ov5647_rd(OV5647_ADDR, 0x0100, &rdval, 1);
  if (ret < 0) {
    return ret;
  }

  ret = ov5647_write_array(ov5647_640x480, ARRAY_SIZE(ov5647_640x480));
  if (ret < 0) {
    DGB_PRINTF(&client->dev, "write sensor default regs error\n");
    return ret;
  }

  ret = ov5647_set_virtual_channel( 0);
  if (ret < 0) {
    return ret;
  }

  ret = ov5647_rd(OV5647_ADDR, 0x0100, &resetval, 1);
  if (ret < 0) {
    return ret;
  }
  if (!(resetval & 0x01)) {
    DGB_PRINTF(&client->dev, "Device was in SW standby");
    ret = ov5647_wr(OV5647_ADDR, 0x0100, 0x01);
    if (ret < 0) {
      return ret;
    }
  }

  return ov5647_wr(OV5647_ADDR, 0x4800, 0x04);
}

/**
 *
 */
static int set_sw_standby(int standby)
{
  int ret;
  unsigned char rdval;

  ret = ov5647_rd(OV5647_ADDR, 0x0100, &rdval,1);
  if (ret < 0) {
    return ret;
  }
  if (standby) {
    rdval &= ~0x01;
  } else {
    rdval |= 0x01;
  }

  return ov5647_wr(OV5647_ADDR, 0x0100, rdval);
}

/**
 *
 */
static int ov5647_write_array(struct regval_list *regs, int array_size)
{
  int i, ret;

  for (i = 0; i < array_size; i++) {
    ret = ov5647_wr(OV5647_ADDR, regs[i].addr, regs[i].data);
    if (ret < 0) {
      return ret;
    }
  }

  return 0;
}

/**
 *
 */
static int ov5647_set_virtual_channel(int channel)
{
  unsigned char channel_id;
  int ret;

  ret = ov5647_rd(OV5647_ADDR, 0x4814, &channel_id, 1);
  if (ret < 0) {
    return ret;
  }
  channel_id &= ~(3 << 6);
  return ov5647_wr(OV5647_ADDR, 0x4814, channel_id | (channel << 6));
}

/**
 *
 */
static int ov5647_wr(alt_u8 address, alt_u16 reg, alt_u8 data)
{
  alt_u8 wr_data[3];
  wr_data[0] = (alt_u8)(reg>>8);
  wr_data[1] = (alt_u8)(reg);
  wr_data[2] = data;

  return i2cWrite(OV5647_I2C, address, wr_data, 3);
}

/**
 *
 */
static int ov5647_rd(alt_u8 address, alt_u16 reg, alt_u8* data, alt_u8 len)
{
  alt_u8 wr_data[2];
  wr_data[0] = (alt_u8)(reg>>8);
  wr_data[1] = (alt_u8)(reg);

  i2cWrite(OV5647_I2C, address, wr_data, 2);

  return i2cRead(OV5647_I2C, address, data, len);
}
