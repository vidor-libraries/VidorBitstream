/*
 * gpio.c
 *
 *  Created on: May 7, 2018
 *      Author: max
 */

#include <stdio.h>
#include <io.h>
#include <system.h>
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
alt_u32 irqPin;
void (*irqHook)(void);

void irqIsr(void* isr_context, alt_u32 id);

/**
 *
 */
alt_u32 irqPinSet(alt_u32 pin, void (*hook)(void))
{
  alt_u32 reg;
  void* context = 0;
  int ret;

  ret = alt_irq_register(IRQ_IRQ, context, irqIsr);
  if (ret) {
    return -1;
  }

  reg = IORD(IRQ_BASE, PIO_DIR);
  reg &=~(1 << pin);
  IOWR(IRQ_BASE, PIO_DIR , reg);

  IOWR(IRQ_BASE, PIO_EDET, 0);

  IOWR(IRQ_BASE, PIO_IMSK, 1 << pin);

  irqPin = pin;
  irqHook = hook;
  return 0;
}

/**
 *
 */
void irqIsr(void* isr_context, alt_u32 id)
{
  if (irqHook) {
    irqHook();
  }
  IOWR(IRQ_BASE, PIO_EDET, 0);
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
