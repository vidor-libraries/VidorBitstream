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
 * @file pwm.h
 * @brief
 */

#include "mb.h"
#include "fpga.h"
#include "pio.h"

alt_u32 IPDiscover(alt_u32 *ipd);
alt_u32 Begin(alt_u32 UID, alt_u32 num, alt_u16* pins);
alt_u32 Assignment(alt_u32* pins);

/**
 * FPGA Subsystem APIs
 *  PID API prototype   Description
 *  1   FPGAVersion     requests FPGA version
 *  2   IPDiscover      Requests FPGA the array of descriptors for all the available IP blocks
 *  3   Begin           locks a channel of the IP initializing it with passed parameters
 */
void fpgaRpc(void)
{
  alt_u32 volatile *rpc = mbPtrGet();
  alt_u32 ret;

  ret = -1;
  switch (RPC_PID(rpc[0])) {
  case 1: ret = FPGA_VER; break;
  case 2: ret = IPDiscover((alt_u32*)&rpc[2]); break;
  case 3: ret = Begin(rpc[1], rpc[2], (alt_u16*)&rpc[3]); break;
  case 4: ret = Assignment((alt_u32*)&rpc[2]); break;
  }
  rpc[1] = ret;
}

/**
 * Requests FPGA the array of descriptors for all the available IP blocks
 */
alt_u32 IPDiscover(alt_u32 *ipd)
{
  int i;

  for (i=0; i<GIID_MAX; i++) {
    ipd[i] = fpgaIp[i].disc;
  }
  return GIID_MAX;
}

/**
 * locks a channel of the IP initializing it with passed parameters
 */
alt_u32 Begin(alt_u32 UID, alt_u32 num, alt_u16* pins)
{
  int i;
  int c;
  int pin_idx[256];
  alt_u16 chn = 0;

  num = num / sizeof(alt_u16);
  for (i=0; i<GIID_MAX; i++) {
    if (RPC_UID_GET(fpgaIp[i].disc) == UID) {
      for (c=0; c<RPC_CHN_GET(fpgaIp[i].disc); c++) {
        // verifica che i pin passati siano compatibili con quelli assegnati all'IP
        int g;
        int p;
        int n;
        int f;

        g = -1;
        f = 1;
        n = 0;
        for (p=0; p<fpgaIp[i].chn[c].npin; p++) {
          if (g != fpgaIp[i].chn[c].pin[p].grp) {
            g = fpgaIp[i].chn[c].pin[p].grp;
            n = 0;
          }
          if (fpgaIp[i].chn[c].pin[p].pin != pins[n]) {
            f = 0;
            break;
          }
          pin_idx[n] = p;
          chn = c;
          n++;
          if (n >= num) {
            break;
          }
        }

        if (f) {
          // verifica che i pin non siano assegnati
          for (n=0; n<num; n++){
            for (p=0; p<FPGA_PINS_NUM; p++){
              if ((fpgaPin[p].port == PIN_PORT(pins[n])) &&
                  (fpgaPin[p].pin  == PIN_PIN(pins[n]))) {
                if (fpgaPin[p].giid || fpgaPin[p].chn || fpgaPin[p].lock) {
                  return -1;
                }
                break;
              }
            }
          }

          // assign pins to giid
          for (n=0; n<num; n++){
            for (p=0; p<FPGA_PINS_NUM; p++){
              if ((fpgaPin[p].port == PIN_PORT(pins[n])) &&
                  (fpgaPin[p].pin  == PIN_PIN(pins[n]))) {
                fpgaPin[p].giid = i;
                fpgaPin[p].chn  = chn;
                fpgaPin[p].lock = 1;
                break;
              }
            }

            // Set pin, direction value mux
            p = fpgaIp[i].chn[chn].pin[pin_idx[n]].pin;
            if (p != NOPIN) {
              pioMode(PIN_PORT(p), PIN_PIN(p), PIN_MUX(p),
                      fpgaIp[i].chn[chn].pin[pin_idx[n]].fid & PIN_DIR_MSK,
                      0);
            }
          }
          return (i<<24) | (c<<12);
        }
      }
    }
  }

  return -1;
}

/**
 */
alt_u32 Assignment(alt_u32* pins)
{
  int i;
  alt_u32 *ptr;

  ptr = (alt_u32*)fpgaPin;
  for (i=0; i<FPGA_PINS_NUM; i++) {
    pins[i] = ptr[i];
  }
  return FPGA_PINS_NUM;
}

/**
 */
alt_u32 fpgaIpQuery(alt_u32 cmd, alt_u32* pins)
{
  alt_u8 giid = RPC_GIID(cmd);
  alt_u16 chn = RPC_CHN(cmd);

  if (RPC_UID_GET(fpgaIp[giid].disc) == PIO_UID) {
    pins[0] = (0 << 24) |           // group
              (1 << 16) |           // function
              PIN(0, giid-1, chn);  // pin mux,port,pin
    return 1;
  } else {
    int p;

    for (p=0; p<fpgaIp[giid].chn[chn].npin; p++) {
      pins[p] = (fpgaIp[giid].chn[chn].pin[p].grp << 24) |
                (fpgaIp[giid].chn[chn].pin[p].fid << 16) |
                 fpgaIp[giid].chn[chn].pin[p].pin;
    }
    return fpgaIp[giid].chn[chn].npin;
  }
}

/**
 */
alt_u32 fpgaIpEnd(alt_u32 cmd)
{
  alt_u8  giid = RPC_GIID(cmd);
  alt_u16 chn = RPC_CHN(cmd);
  int     p;

  for (p=0; p<FPGA_PINS_NUM; p++){
    if ((fpgaPin[p].giid == giid) && (fpgaPin[p].chn == chn)) {
      fpgaPin[p].giid = 0;
      fpgaPin[p].chn  = 0;
      fpgaPin[p].lock = 0;
      // set pin mux to 0, dir to input
      pioMode(fpgaPin[p].port, fpgaPin[p].pin, 0, 0, 0);
    }
  }

  return 0;
}
