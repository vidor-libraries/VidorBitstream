/**
 *
 */
#include "mb.h"
#include "rpc.h"

#define SEC_RAM  __attribute__((__section__(".rwdata")))

/**
 *
 */
int SEC_RAM main(void)
{
  int i;

  while (1) {
    if (mbMsgRx()) {
      rpcCmd();
    }
    for (i=0; i<LOOP_MAX; i++) {
      fpgaIpLoop[i].loop();
    }
  }
  return 0;
}

