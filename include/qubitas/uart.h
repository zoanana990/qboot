#ifndef __UART_H__
#define __UART_H__

#include <qubitas/type.h>
#include <qubitas/utils.h>

/* USART initialization */
void usart_init(void);

/* USART reset */
void usart_reset(void);

/* send n-bytes data */
void usart_send_data(u8 *pTxBuffer, u8 len);

/* receive 1 byte data */
u8 usart_receive_data(void);

#endif