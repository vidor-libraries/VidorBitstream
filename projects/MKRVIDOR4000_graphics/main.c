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
  mbInit();

  while (1) {
    if (mbMsgRx()) {
      rpcCmd();
    }
  };

  return 0;
}

