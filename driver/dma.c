#include <qubitas/dma.h>
#include <qubitas/type.h>
#include <qubitas/io.h>
#include <qubitas/utils.h>
#include <qubitas/nvic.h>
#include <qubitas/printk.h>

#define RCC_BASE                    (0x40023800UL)
#define RCC_AHB1ENR                 (RCC_BASE + 0x30UL)
#define RCC_APB1ENR                 (RCC_BASE + 0x40UL)
#define RCC_APB1RSTR                (RCC_BASE + 0x20UL)

#define RCC_USART3_PIN              (18)
#define RCC_DMA1_EN_PIN             (21)

#define DMA1_BASE                   (0x40026000UL)
#define DMA2_BASE                   (0x40026499UL)
#define DMA2D_BASE                  (0x4002B000UL)
#define DMA_LISR(base)              (base + 0x00UL)
#define DMA_LIFCR(base)             (base + 0x08UL)
#define DMA_SxCR(base)              (base + 0x10UL)
#define DMA_SxNDTR(base)            (base + 0x14UL)
#define DMA_SxPAR(base)             (base + 0x18UL)
#define DMA_SxM0AR(base)            (base + 0x1CUL)
#define DMA_SxFCR(base)             (base + 0x24UL)

#define DMA_LISR_FEIF1_BIT          (6)
#define DMA_LISR_DMEIF1_BIT         (8)
#define DMA_LISR_TEIF1_BIT          (9)
#define DMA_LISR_HTIF1_BIT          (10)
#define DMA_LISR_TCIF1_BIT          (11)

#define DMA_LIFCR_FEIF1_BIT         (6)
#define DMA_LIFCR_DMEIF1_BIT        (8)
#define DMA_LIFCR_TEIF1_BIT         (9)
#define DMA_LIFCR_HTIF1_BIT         (10)
#define DMA_LIFCR_TCIF1_BIT         (11)

#define DMA_LISR_FEIF3_BIT          (22)
#define DMA_LISR_DMEIF3_BIT         (24)
#define DMA_LISR_TEIF3_BIT          (25)
#define DMA_LISR_HTIF3_BIT          (26)
#define DMA_LISR_TCIF3_BIT          (27)

#define DMA_LIFCR_FEIF3_BIT         (22)
#define DMA_LIFCR_DMEIF3_BIT        (24)
#define DMA_LIFCR_TEIF3_BIT         (25)
#define DMA_LIFCR_HTIF3_BIT         (26)
#define DMA_LIFCR_TCIF3_BIT         (27)

#define DMA_CR_EN_BIT               (0)
#define DMA_CR_DMEIE_BIT            (1)
#define DMA_CR_TEIE_BIT             (2)
#define DMA_CR_HTIE_BIT             (3)
#define DMA_CR_TCIE_BIT             (4)
#define DMA_CR_DIR_BIT              (6)
#define DMA_CR_MINC_BIT             (10)
#define DMA_CR_PSIZE_BIT            (11)
#define DMA_CR_MSIZE_BIT            (13)
#define DMA_CR_CHSEL_BIT            (25)

#define DMA_FCR_FTH_BIT             (0)
#define DMA_FCR_DMDIS_BIT           (2)
#define DMA_FCR_FEIE_BIT            (7)

#define DMA_IRQ_BASE_NO             (11)

#define DMA_IRQ_STREAM7             (47)

#define USART3_BASE                 (0x40004800UL)
#define USART_TDR(base)             (base + 0x28UL)
#define USART_RDR(base)             (base + 0x24UL)
#define USART_CR3(base)             (base + 0x08UL)

#define USART_CR3_DMAR_BIT          (6)
#define USART_CR3_DMAT_BIT          (7)

char DMA_TX_DATA_STREAM[DMA_MAX_STRLEN];
char DMA_RX_DATA_STREAM[DMA_MAX_STRLEN];

void dma_htRxCallback(void) {

}

void dma_ftRxCallback(void) {
    u32 base = dma_getStreamBase(DMA1, 3);
    io_write(DMA_SxNDTR(base), DMA_MAX_STRLEN);

    io_writeMask(USART_CR3(USART3_BASE), 1 << USART_CR3_DMAR_BIT,
                 1 << USART_CR3_DMAR_BIT);

    printk("%s\n", DMA_RX_DATA_STREAM);

    dma_enStreamX(DMA1, DMA_STREAM1);
}

void dma_htCallback(void) {
    u32 base = dma_getStreamBase(DMA1, 3);
    io_write(DMA_SxNDTR(base), DMA_MAX_STRLEN);

    io_writeMask(USART_CR3(USART3_BASE), 0 << USART_CR3_DMAT_BIT,
                 1 << USART_CR3_DMAT_BIT);

    dma_enStreamX(DMA1, DMA_STREAM3);
}

void dma_ftCallback(void) {
    u32 base = dma_getStreamBase(DMA1, 3);
    io_write(DMA_SxNDTR(base), DMA_MAX_STRLEN);

    io_writeMask(USART_CR3(USART3_BASE), 0 << USART_CR3_DMAT_BIT,
                 1 << USART_CR3_DMAT_BIT);

    dma_enStreamX(DMA1, DMA_STREAM3);
}

void dma_teCallback(void) {
    while (1);
}

void dma_dmeCallback(void) {
    while (1);
}

void dma_feCallback(void) {
    while (1);
}

u32 dma_getBase(u32 dma_channel) {
    if (dma_channel == DMA1)
        return DMA1_BASE;
    else if (dma_channel == DMA2)
        return DMA2_BASE;
    else
        return DMA2D_BASE;
}

u32 dma_getStreamBase(u32 dma_channel, u32 stream) {
    u32 base;
    if (dma_channel == DMA1)
        base = DMA1_BASE;
    else if (dma_channel == DMA2)
        base = DMA2_BASE;
    else if (dma_channel == DMA2D)
        base = DMA2D_BASE;
    else
        return 0;
    return (base + 0x18 * stream);
}

void dma_enStreamX(u32 dma_channel, u32 stream) {
    u32 base = dma_getStreamBase(dma_channel, stream);

    io_writeMask(DMA_SxCR(base), 1 << DMA_CR_EN_BIT,
                 MASK(1) << DMA_CR_EN_BIT);
}

void dma_configIntr(u32 dma_channel, u32 stream) {
    u32 base = dma_getStreamBase(dma_channel, stream);
    u32 irq_no = ((stream == 7) ? DMA_IRQ_STREAM7 : (stream + DMA_IRQ_BASE_NO));

    /* 1. Config Half-transfer IE (HTIE) */
    io_writeMask(DMA_SxCR(base), 1 << DMA_CR_HTIE_BIT,
                 MASK(1) << DMA_CR_HTIE_BIT);

    /* 2. Transfer complete IE (TCIE) */
    io_writeMask(DMA_SxCR(base), 1 << DMA_CR_TCIE_BIT,
                 MASK(1) << DMA_CR_TCIE_BIT);

    /* 3. Transfer error IE (TEIE) */
    io_writeMask(DMA_SxCR(base), 1 << DMA_CR_TEIE_BIT,
                 MASK(1) << DMA_CR_TEIE_BIT);

    /* 4. FIFO overrun/underrun IE (FEIE) */
    io_writeMask(DMA_SxFCR(base), 1 << DMA_FCR_FEIE_BIT,
                 MASK(1) << DMA_FCR_FEIE_BIT);

    /* 5. Direct mode error (DMEIE) */
    io_writeMask(DMA_SxCR(base), 1 << DMA_CR_DMEIE_BIT,
                 MASK(1) << DMA_CR_DMEIE_BIT);

    /* 6. Enable the IRQ for DMA1 stream global interrupt in NVIC */
    nvic_enIrq(irq_no);
}

void dma_initUart3Tx(void) {
    u32 base;
    /* 1. enable the peripheral clock for the dma
     *    AHB1 Bus, RCC_AHB1Bus enable
     * */
    io_writeMask(RCC_AHB1ENR, 1 << RCC_DMA1_EN_PIN,
                 MASK(1) << RCC_DMA1_EN_PIN);

    /* 2. identify the stream which is suitable for your peripheral
     *    - In STM32F746
     *      USART3_TX: Stream 3, Channel 4
     *      USART3_RX: Stream 1, Channel 4
     *    - Notice that, the register address = address + 0x18 * stream number
     * */
    base = dma_getStreamBase(DMA1, 3);
    io_writeMask(DMA_SxCR(base), 4 << DMA_CR_CHSEL_BIT,
                 MASK(3) << DMA_CR_CHSEL_BIT);

    /* 3. Identify the channel number on which uart3 peripheral serial */

    /* 4. Program the source address (memory) */
    io_write(DMA_SxM0AR(base), (u32) DMA_TX_DATA_STREAM);

    /* 5. Program the destination address (peripheral) */
    io_write(DMA_SxPAR(base), (u32) USART_TDR(USART3_BASE));

    /* 6. Program the number of data items to send */
    io_write(DMA_SxNDTR(base), DMA_MAX_STRLEN);

    /* 7.   The direction of data transfer, m2p, p2m, m2m
     * 7.1. We config the direction of DMA, that is, memory to peripheral
     * */
    io_writeMask(DMA_SxCR(base), 1 << DMA_CR_DIR_BIT,
                 MASK(2) << DMA_CR_DIR_BIT);

    /* 8. Program the source and destination data width
     * 8.1. We need to config the memory data size for DMA
     * 8.2. We need to config the peripheral data size for DMA
     * 8.3. Enable the memory auto-increment
     * */
    io_writeMask(DMA_SxCR(base), 0 << DMA_CR_MSIZE_BIT,
                 MASK(2) << DMA_CR_MSIZE_BIT);
    io_writeMask(DMA_SxCR(base), 0 << DMA_CR_PSIZE_BIT,
                 MASK(2) << DMA_CR_PSIZE_BIT);
    io_writeMask(DMA_SxCR(base), 1 << DMA_CR_MINC_BIT,
                 MASK(1) << DMA_CR_MINC_BIT);

    /* 9. Direct mode or fifo mode */
    io_writeMask(DMA_SxFCR(base), 1 << DMA_FCR_DMDIS_BIT,
                 MASK(1) << DMA_FCR_DMDIS_BIT);

    /* 10. Select the fifo threshold */
    io_writeMask(DMA_SxFCR(base), 3 << DMA_FCR_FTH_BIT,
                 MASK(2) << DMA_FCR_FTH_BIT);
}

void dma_initUart3Rx(void) {
    u32 base;
    /* 1. enable the peripheral clock for the dma
     *    AHB1 Bus, RCC_AHB1Bus enable
     * */
    io_writeMask(RCC_AHB1ENR, 1 << RCC_DMA1_EN_PIN,
                 MASK(1) << RCC_DMA1_EN_PIN);

    /* 2. identify the stream which is suitable for your peripheral
     *    - In STM32F746
     *      USART3_TX: Stream 3, Channel 4
     *      USART3_RX: Stream 1, Channel 4
     *    - Notice that, the register address = address + 0x18 * stream number
     * */
    base = dma_getStreamBase(DMA1, 1);
    io_writeMask(DMA_SxCR(base), 4 << DMA_CR_CHSEL_BIT,
                 MASK(3) << DMA_CR_CHSEL_BIT);

    /* 3. Identify the channel number on which uart3 peripheral serial */

    /* 4. Program the source address (memory) */
    io_write(DMA_SxM0AR(base), (u32) DMA_RX_DATA_STREAM);

    /* 5. Program the destination address (peripheral) */
    io_write(DMA_SxPAR(base), (u32) USART_RDR(USART3_BASE));

    /* 6. Program the number of data items to send */
    io_write(DMA_SxNDTR(base), DMA_MAX_STRLEN);

    /* 7.   The direction of data transfer, m2p, p2m, m2m
     * 7.1. We config the direction of DMA, that is, peripheral to memory
     * */
    io_writeMask(DMA_SxCR(base), 0 << DMA_CR_DIR_BIT,
                 MASK(2) << DMA_CR_DIR_BIT);

    /* 8. Program the source and destination data width
     * 8.1. We need to config the memory data size for DMA
     * 8.2. We need to config the peripheral data size for DMA
     * 8.3. Enable the memory auto-increment
     * */
    io_writeMask(DMA_SxCR(base), 0 << DMA_CR_MSIZE_BIT,
                 MASK(2) << DMA_CR_MSIZE_BIT);
    io_writeMask(DMA_SxCR(base), 0 << DMA_CR_PSIZE_BIT,
                 MASK(2) << DMA_CR_PSIZE_BIT);
    io_writeMask(DMA_SxCR(base), 1 << DMA_CR_MINC_BIT,
                 MASK(1) << DMA_CR_MINC_BIT);

    /* 9. Direct mode or fifo mode */
    io_writeMask(DMA_SxFCR(base), 1 << DMA_FCR_DMDIS_BIT,
                 MASK(1) << DMA_FCR_DMDIS_BIT);

    /* 10. Select the fifo threshold */
    io_writeMask(DMA_SxFCR(base), 3 << DMA_FCR_FTH_BIT,
                 MASK(2) << DMA_FCR_FTH_BIT);

    io_writeMask(USART_CR3(USART3_BASE), 1 << USART_CR3_DMAR_BIT,
                 1 << USART_CR3_DMAR_BIT);
}

/* IRQ handler for DMA1 stream3 */
void DMA1_Stream3_IRQHandler(void) {
    u32 base = dma_getBase(DMA1);
    if ((io_read(DMA_LISR(base)) >> DMA_LISR_HTIF3_BIT) & 1) {
        io_write(DMA_LIFCR(base), 1 << DMA_LIFCR_HTIF3_BIT);
        dma_htCallback();
    } else if ((io_read(DMA_LISR(base)) >> DMA_LISR_TCIF3_BIT) & 1) {
        io_write(DMA_LIFCR(base), 1 << DMA_LIFCR_TCIF3_BIT);
        dma_ftCallback();
    } else if ((io_read(DMA_LISR(base)) >> DMA_LISR_TEIF3_BIT) & 1) {
        io_write(DMA_LIFCR(base), 1 << DMA_LIFCR_TEIF3_BIT);
        dma_teCallback();
    } else if ((io_read(DMA_LISR(base)) >> DMA_LISR_FEIF3_BIT) & 1) {
        io_write(DMA_LIFCR(base), 1 << DMA_LIFCR_FEIF3_BIT);
        dma_feCallback();
    } else if ((io_read(DMA_LISR(base)) >> DMA_LISR_DMEIF3_BIT) & 1) {
        io_write(DMA_LIFCR(base), 1 << DMA_LIFCR_DMEIF3_BIT);
        dma_dmeCallback();
    } else
        return;
}


void DMA1_Stream1_IRQHandler(void) {
    u32 base = dma_getBase(DMA1);
    if ((io_read(DMA_LISR(base)) >> DMA_LISR_HTIF1_BIT) & 1) {
        io_write(DMA_LIFCR(base), 1 << DMA_LIFCR_HTIF1_BIT);
        dma_htRxCallback();
    } else if ((io_read(DMA_LISR(base)) >> DMA_LISR_TCIF1_BIT) & 1) {
        io_write(DMA_LIFCR(base), 1 << DMA_LIFCR_TCIF1_BIT);
        dma_ftRxCallback();
    } else if ((io_read(DMA_LISR(base)) >> DMA_LISR_TEIF1_BIT) & 1) {
        io_write(DMA_LIFCR(base), 1 << DMA_LIFCR_TEIF1_BIT);
        dma_teCallback();
    } else if ((io_read(DMA_LISR(base)) >> DMA_LISR_FEIF1_BIT) & 1) {
        io_write(DMA_LIFCR(base), 1 << DMA_LIFCR_FEIF1_BIT);
        dma_feCallback();
    } else if ((io_read(DMA_LISR(base)) >> DMA_LISR_DMEIF1_BIT) & 1) {
        io_write(DMA_LIFCR(base), 1 << DMA_LIFCR_DMEIF1_BIT);
        dma_dmeCallback();
    } else
        return;
}
