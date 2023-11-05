#ifndef __UART_H__
#define __UART_H__

#include <qubitas/type.h>
#include <qubitas/utils.h>

#define DEFAULT_FCLK                (16000000U)
#define BAUDRATE                    (115200U)

/* AHB1 peripheral clock enable register */
#define RCC_AHB1ENR *(__vo u32 *)   (0x40023830U)
/* APB1 peripheral reset register */
#define RCC_APB1RSTR *(__vo u32 *)  (0x40023820U)
/* APB1 peripheral clock enable register */
#define RCC_APB1ENR *(__vo u32 *)   (0x40023840U)

/* GPIO port mode register */
#define GPIOD_MODE *(__vo u32 *)    (0x40020C00U)
/* GPIO alternate function high register */
#define GPIOD_AFRH *(__vo u32 *)    (0x40020C24U)

/* control register 1 */
#define USART3_CR1 *(__vo u32 *)    (0x40004800U)
/* baudrate register */
#define USART3_BRR *(__vo u32 *)    (0x4000480CU)
/* Request register */
#define USART3_RQR *(__vo u32 *)    (0x40004818U)
/* Interrupt and status register */
#define USART3_ISR *(__vo u32 *)    (0x4000481CU)
/* Receive data register */
#define USART3_RDR *(__vo u32 *)    (0x40004824U)
/* Transmit data register */
#define USART3_TDR *(__vo u32 *)    (0x40004828U)

/* USART initialization */
void usart_init(void);

/* USART reset */
void usart_reset(void);

/* send n-bytes data */
void usart_send_data(u8 *pTxBuffer, u8 len);

/* receive 1 byte data */
u8 usart_receive_data(void);

#endif