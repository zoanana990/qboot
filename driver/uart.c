#include <uart.h>

/* USART initialization */
void usart_init(void)
{
    /***************** GPIO set *****************/
    /* GPIOD RCC enable */
    RCC_AHB1ENR |= 1 << 3;
    /* PD8 PD9 set alternate function mode */
    GPIOD_MODE &= ~(0xF << 16);
    GPIOD_MODE |= (0x2 << 16) | (0x2 << 18);
    /* PD8: USART3 Tx | PD9: USART3 Rx */
    GPIOD_AFRH &= ~(0xFF << 0);
    GPIOD_AFRH |= 0x77 << 0;

    /***************** USART Set *****************/
    /* USART RCC enable */
    RCC_APB1ENR |= 1 << 18;
    /* UE (USART Enable) set */
    USART3_CR1 |= 1 << 0;
    /* Enable Tx/Rx */
    USART3_CR1 |= (1 << 2) | (1 << 3);
    /* Baudrate setup */
    USART3_BRR = DEFAULT_FCLK / BAUDRATE;
}

/* USART reset */
void usart_reset(void)
{
    RCC_APB1RSTR |= 1 << 18;
    RCC_APB1RSTR &= ~(1 << 18);
}

/* send n-bytes data */
void usart_send_data(u8 *pTxBuffer, u8 len)
{
    for (u8 i = 0; i < len; i++) {
        /* waiting for the transmit data register empty (bit[7] TXE) */
        while (!(USART3_ISR & 0x80));
        /* send data */
        USART3_TDR = (*pTxBuffer++ & 0xFF);
        /* waiting for the transmit complete (bit[6] TC) */
        while (!(USART3_ISR & 0x40));
    }
}

/* receive 1 byte data */
u8 usart_receive_data(void)
{
    u8 data;
    /* waiting for the transmit data transmit to USART_RDR register
     * (USART_ISR bit[5] RXNE) */
    while (!(USART3_ISR & 0x20));
    /* receive data */
    data = USART3_RDR;
    /* clear RXNE by USARTx_RQR (bit[3] RXFRQ) */
    USART3_RQR |= 1 << 3;
    return data;
}