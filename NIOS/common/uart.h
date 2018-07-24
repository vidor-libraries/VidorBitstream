/*
 * uart.h
 *
 *  Created on: May 8, 2018
 *      Author: max
 */

#ifndef UART_H_
#define UART_H_

#define UART_PARITY_EVEN (0x1ul)
#define UART_PARITY_ODD  (0x2ul)
#define UART_PARITY_NONE (0x3ul)
#define UART_PARITY_MASK (0xFul)

#define UART_STOP_BIT_1    (0x10ul)
#define UART_STOP_BIT_1_5  (0x20ul)
#define UART_STOP_BIT_2    (0x30ul)
#define UART_STOP_BIT_MASK (0xF0ul)

#define UART_DATA_5    (0x100ul)
#define UART_DATA_6    (0x200ul)
#define UART_DATA_7    (0x300ul)
#define UART_DATA_8    (0x400ul)
#define UART_DATA_MASK (0xF00ul)

void uartInit(int devs);
void uartCmd(void);

alt_u32 uartEnable(alt_u32 idx, alt_u32 mode);
alt_u32 uartSet(alt_u32 idx, alt_u32 baud, alt_u32 config);
alt_u32 uartDisable(alt_u32 idx);
alt_u32 uartGet(alt_u32 idx);
alt_u32 uartRead(alt_u32 idx, alt_u8* buf, alt_u32 len);
alt_u32 uartAvail(alt_u32 idx);
alt_u32 uartPut(alt_u32 idx, alt_u8 data);
alt_u32 uartWrite(alt_u32 idx, alt_u8* buf, alt_u32 len);
alt_u32 uartFlush(alt_u32 idx);

#endif /* UART_H_ */
