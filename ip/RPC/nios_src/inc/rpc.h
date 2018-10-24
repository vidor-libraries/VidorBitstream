/**
 * Copyright 2018 ARDUINO SA (http://www.arduino.cc/)
 * This file is part of Vidor IP.
 * Copyright (c) 2018
 * Authors: Massimiliano Agneni
 *
 * This software is released under:
 * The GNU General Public License, which covers the main part of Vidor IP
 * The terms of this license can be found at:
 * https://www.gnu.org/licenses/gpl-3.0.en.html
 *
 * You can be released from the requirements of the above licenses by purchasing
 * a commercial license. Buying such a license is mandatory if you want to modify or
 * otherwise use the software for commercial activities involving the Arduino
 * software without disclosing the source code of your own applications. To purchase
 * a commercial license, send an email to license@arduino.cc.
 *
 * @file rpc.h
 * @brief Remote Procedure Call
 */
#ifndef __RPC_H
#define __RPC_H

#include <alt_types.h>

/**
 * Discovery
 * Return value (alt_u32[])
 *
 * 31:20             19:0
 * Channel number    UID
 */
#define IP_DISC(c,u) ((alt_u32)((((c)&0xFFF)<<20)|((u)&0xFFFFF)))

#define RPC_UID_GET(d) ((d)&0xFFFFF)
#define RPC_CHN_GET(d) (((d)>>20)&0xFFF)

/**
 * Pin definition (alt_u16)
 * 15:11       10:5          4:0
 * MUX index   Port number   Pin number
*/
#define PIN(m,p,n) ((alt_u16)((((m)&0x1F)<<11)|(((p)&0x3F)<<5)|((n)&0x1F)))

#define NOPIN      0xF800

#define PIN_MUX(p)  ((p>>11)&0x1F)
#define PIN_PORT(p) ((p>>5)&0x3F)
#define PIN_PIN(p)  ((p>>0)&0x1F)


/**
 * Pins number definition
 */
#define NPIN(v) (sizeof(v)/sizeof(sFpgaIpPin))

#define PIN_DIR_I   0x00
#define PIN_DIR_O   0x80
#define PIN_DIR_MSK 0x80
#define PIN_FNC(f,d) (((f)&0x7F)|((d)&0x80))

/**
 * FPGA's pin assignment structure
 */
typedef struct {
  alt_u32 port: 6;
  alt_u32 pin : 5;
  alt_u32 giid: 8;
  alt_u32 chn :12;
  alt_u32 lock: 1;
} sFpgaPin, *psFpgaPin;


/**
 * RPC function pointer and version for each IP structure
 */
typedef struct {
  void (*rpc)(void);
  alt_u32     ver;
} sFpgaIpRpc, *psFpgaIpRpc;

/**
 * IP's pin definition structure
 */
typedef struct {
  alt_u8   grp;
  alt_u8   fid;
  alt_u16  pin;
} sFpgaIpPin, *psFpgaIpPin;

/**
 * IP's channel definition structure
 */
typedef struct {
  alt_u32     npin;
  psFpgaIpPin pin;
} sFpgaIpChn, *psFpgaIpChn;

/**
 * IP definition structure
 */
typedef struct {
  alt_u8      rpc;
  alt_u32     disc;
  alt_u32     base;
  psFpgaIpChn chn;
  void       *priv;
} sFpgaIp, *psFpgaIp;

/**
 * RPC command defines
 *
 * 31:24  23:12  11:0
 * giid   chn    prcid
 *
 * giid   Global Instance ID
 * chn    Channel
 * prcid  Procedure ID
 *
 */
#define RPC_GIID(a)   (((a)>>24)&0xFF)
#define RPC_CHN(a)    (((a)>>12)&0xFFF)
#define RPC_PID(a)    (((a)>> 0)&0xFFF)

/**
 * Error codes definition
 */
#define RPC_ERR_GIID_INV  0xFFFFFF00

void rpcCmd(void);

#endif /* __RPC_H */
