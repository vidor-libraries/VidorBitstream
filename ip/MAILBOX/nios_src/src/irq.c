/*
 * irq.c
 *
 *  Created on: Aug 29, 2018
 *      Author: max
 */

#include "config.h"


#define SEC_RAM  __attribute__((__section__(".rwdata")))

#include <alt_types.h>
#include <io.h>
#include <sys/alt_irq.h>

/* PIO registers */
#define PIO_DATA  0x00    // data value
#define PIO_DIR   0x01    // direction
#define PIO_IMSK  0x02    // interrupt mask
#define PIO_EDET  0x03    // edge detection
#define PIO_SET   0x04    // bit of output port to set
#define PIO_CLR   0x05    // bit of output port to clear

/**
 *
 */
void (*irqHook)(void);

#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
void irqIsr(void* isr_context);
#else
void irqIsr(void* isr_context, alt_u32 id);
#endif

/**
 *
 */
alt_u32 SEC_RAM irqPinSet(alt_u32 pin, void (*hook)(void))
{
  alt_u32   reg;
//  void     *context = (void*)pin;
//  int       ret;
/*
  if (hook) {
#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
    ret = alt_ic_isr_register(IRQ_IRQ_INTERRUPT_CONTROLLER_ID,
                              IRQ_IRQ,
                              irqIsr, context, 0);
#else
    ret = alt_irq_register(IRQ_IRQ, context, irqIsr);
#endif
  } else {
#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
    ret = alt_ic_irq_disable(IRQ_IRQ_INTERRUPT_CONTROLLER_ID, IRQ_IRQ);
#else
    ret = alt_irq_register(IRQ_IRQ, NULL, NULL);
#endif
  }
  if (ret) {
    return -1;
  }
*/
  reg = IORD(IRQ_BASE, PIO_DIR);
  reg &=~(1 << pin);
  IOWR(IRQ_BASE, PIO_DIR , reg);

  IOWR(IRQ_BASE, PIO_EDET, 1 << pin);

  IOWR(IRQ_BASE, PIO_IMSK, 1 << pin);

  irqHook = hook;
  return 0;
}

/**
 *
 */
#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
void SEC_RAM irqIsr(void* isr_context)
#else
void SEC_RAM irqIsr(void* isr_context, alt_u32 id)
#endif
{
  alt_u32 pin = (alt_u32)isr_context;
  if (irqHook) {
    irqHook();
  }
  IOWR(IRQ_BASE, PIO_EDET, 1 << pin);
}

/**
 */
alt_u32 SEC_RAM irqPinDet(alt_u32 pin)
{
  return (IORD(IRQ_BASE, PIO_EDET) & (1 << pin));
}

/**
 */
alt_u32 SEC_RAM irqPinClr(alt_u32 pin)
{
  IOWR(IRQ_BASE, PIO_EDET, 1 << pin);
  return 0;
}

/**
 */
int intPinInit(alt_u32 pin, int val)
{
  alt_u32 reg;

  reg = IORD(IRQ_BASE, PIO_DIR);
  reg |= (1 << pin);
  IOWR(IRQ_BASE, PIO_DIR , reg);

  if (val) {
    IOWR(IRQ_BASE, PIO_SET, (1 << pin));
  }else{
    IOWR(IRQ_BASE, PIO_CLR, (1 << pin));
  }
  return 0;
}

/**
 */
int intPinSet(alt_u32 pin, int val)
{
  if (val) {
    IOWR(IRQ_BASE, PIO_SET, (1 << pin));
  }else{
    IOWR(IRQ_BASE, PIO_CLR, (1 << pin));

  }
  return 0;
}

/**
 */
int intPinGet(alt_u32 pin)
{
  return IORD(IRQ_BASE, PIO_DATA) & (1 << pin);;
}
