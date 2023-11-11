// This file is used for STM32F746 device
#include <qubitas/uart.h>
#include <qubitas/type.h>
#include <qubitas/gpio.h>
#include <qubitas/io.h>
#include <qubitas/printk.h>
#include <qubitas/dma.h>
#include <qubitas/nvic.h>

#define DEFAULT_FCLK                (16000000U)
#define BAUDRATE                    (115200U)

#define RCC_BASE                    (0x40023800UL)
#define RCC_AHB1ENR                 (RCC_BASE + 0x30UL)
#define RCC_APB1ENR                 (RCC_BASE + 0x40UL)
#define RCC_APB1RSTR                (RCC_BASE + 0x20UL)

#define RCC_USART3_PIN              (18)
#define RCC_DMA1_EN_PIN             (21)

#define USART3_BASE                 (0x40004800UL)

#define USART_CR1(base)             (base + 0x00UL)
#define USART_CR2(base)             (base + 0x04UL)
#define USART_CR3(base)             (base + 0x08UL)
#define USART_BRR(base)             (base + 0x0CUL)
#define USART_RQR(base)             (base + 0x18UL)
#define USART_ISR(base)             (base + 0x1CUL)
#define USART_ICR(base)             (base + 0x20UL)
#define USART_RDR(base)             (base + 0x24UL)
#define USART_TDR(base)             (base + 0x28UL)

#define USART3_TX                   (8)
#define USART3_RX                   (9)

#define USART_CR1_EN_BIT            (0)
#define USART_CR1_RE_BIT            (2)
#define USART_CR1_TE_BIT            (3)
#define USART_CR1_RXNEIE_BIT        (5)
#define USART_CR1_RTOIE_BIT         (26)

#define USART_CR2_RTOEN_BIT         (23)

#define USART_CR3_EIE_BIT           (0)
#define USART_CR3_DMAR_BIT          (6)

#define USART_ISR_TXE_BIT           (7)
#define USART_ISR_TXC_BIT           (6)
#define USART_ISR_RXNE_BIT          (5)

#define USART_ICR_IDLECF_BIT        (4)
#define USART_ICR_TCCF_BIT          (6)
#define USART_ICR_RTOCF_BIT         (11)

#define USART_RQR_RXFRQ_BIT         (3)

#define USART3_IRQ_NO               (39)

extern char DMA_TX_DATA_STREAM[DMA_MAX_STRLEN];

/* USART3 initialization */
void usart_init(void) {
    u32 base = USART3_BASE;
    u32 val;
    u32 mask;

    /* 1. Enable the peripheral clock for the USART3 Peripheral */
    val = 1 << RCC_USART3_PIN;
    mask = MASK(1) << RCC_USART3_PIN;
    io_writeMask(RCC_APB1ENR, val, mask);

    /* 2. Configure the gpio pins for uart tx/rx
     * */

    /* 2.1. GPIOD RCC enable */
    gpio_init(GPIOD);

    /* 2.2. PD8 PD9 set alternate function mode */
    gpio_setMode(GPIOD, USART3_TX, GPIO_ALTERNATE);
    gpio_setMode(GPIOD, USART3_RX, GPIO_ALTERNATE);

    /* 2.3. PD8: USART3 Tx | PD9: USART3 Rx */
    gpio_setAltFunc(GPIOD, USART3_TX, GPIO_FUNC_USART);
    gpio_setAltFunc(GPIOD, USART3_RX, GPIO_FUNC_USART);

    /* 2.4. Set the pull up to the PD8 PD9*/
    gpio_setPuPd(GPIOD, USART3_TX, GPIO_OUTPUT_Pull_UP);
    gpio_setPuPd(GPIOD, USART3_RX, GPIO_OUTPUT_Pull_UP);

    /* 3. Baudrate setup */
    io_write(USART_BRR(base), (DEFAULT_FCLK / BAUDRATE) + 1);

    /* 4. Configure the data width, no of stop bits etc.
     *    <Default configuration meets our requirement>
     * */

    /* 5. USART Enable
     * */
    val = 1 << USART_CR1_EN_BIT;
    mask = MASK(1) << USART_CR1_EN_BIT;
    io_writeMask(USART_CR1(base), val, mask);

    /* 6. USART Tx enable and Rx enable
     * */
    val = (1 << USART_CR1_RE_BIT) | (1 << USART_CR1_TE_BIT);
    mask = (MASK(1) << USART_CR1_RE_BIT) | (MASK(1) << USART_CR1_TE_BIT);
    io_writeMask(USART_CR1(base), val, mask);

    /* 7. Enable RXNE interrupt */
    io_writeBit(USART_CR1(base), USART_CR1_RXNEIE_BIT);
    nvic_enIrq(USART3_IRQ_NO);
}

/* USART reset */
void usart_reset(void) {
    io_writeMask(RCC_APB1RSTR, 1 << RCC_USART3_PIN, 1 << RCC_USART3_PIN);
    io_writeMask(RCC_APB1RSTR, 0 << RCC_USART3_PIN, 1 << RCC_USART3_PIN);
}

/* send n-bytes data */
void usart_txByte(s8 c) {
    /* 1. Make sure the interrupt and status register, the TXE(bit[7]) is set
     *    If TXE is 1, put the byte
     *    Here, we are waiting for the io_read value
     * */
    while (!(io_read(USART_ISR(USART3_BASE)) & (MASK(1) << USART_ISR_TXE_BIT)));

    /* 2. write the data to uart tx data register
     * */
    io_write(USART_TDR(USART3_BASE), c & MASK(8));

    /* 3. waiting for the tx transmission complete (TXC)
     * */
    while (!(io_read(USART_ISR(USART3_BASE)) & (MASK(1) << USART_ISR_TXC_BIT)));
}

/* receive 1 byte data */
u8 usart_rxByte(void) {
    u8 data;
    u32 base = USART3_BASE;
    /* 1. Waiting for the transmit data to USART_RDR register
     *    (USART_ISR bit[5] RXNE)
     * */
    while (!(io_read(USART_ISR(base)) & (MASK(1) << USART_ISR_RXNE_BIT)));

    /* 2. Receive data */
    data = (u8) io_read(USART_RDR(base)) & MASK(8);

    /* 3. Clear the RXNE by USART_RQR (bit[3]) */
    io_write(USART_RQR(base), MASK(1) << USART_RQR_RXFRQ_BIT);

    return data;
}

/* send n-bytes data */
void usart_txData(u8 *ptr_tx_buffer) {
    while (*(ptr_tx_buffer)) {
        usart_txByte(*(ptr_tx_buffer++));
    }
}

void put_c(char c) {
    usart_txByte(c);
    if (c == '\n') {
        usart_txData((u8 *) "\r");
    }
}

void uart_receiveDma(void) {
    /* TODO: USE DMA method */
}

void USART3_IRQHandler(void) {
    u8 data = usart_rxByte();

    /* TODO: command line parser */
    switch (data) {
        case '\r':
            printk("\r\n");
            break;
        case '\b':
            put_c(data);
            put_c(' ');
            put_c(data);
            break;
        default:
            put_c(data);
            break;
    }
}