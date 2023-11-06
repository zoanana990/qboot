#include <qubitas/dma.h>
#include <qubitas/type.h>
#include <qubitas/io.h>
#include <qubitas/utils.h>

#define RCC_BASE                    (0x40023800UL)
#define RCC_AHB1ENR                 (RCC_BASE + 0x30UL)
#define RCC_APB1ENR                 (RCC_BASE + 0x40UL)
#define RCC_APB1RSTR                (RCC_BASE + 0x20UL)

#define RCC_USART3_PIN              (18)
#define RCC_DMA1_EN_PIN             (21)

#define USART3_BASE                 (0x40004800UL)
#define USART_TDR(base)             (base + 0x28UL)

#define DMA1_BASE                   (0x40026000UL)
#define DMA2_BASE                   (0x40026499UL)
#define DMA2D_BASE                  (0x4002B000UL)
#define DMA_SxCR(base)              (base + 0x10UL)
#define DMA_SxNDTR(base)            (base + 0x14UL)
#define DMA_SxPAR(base)             (base + 0x18UL)
#define DMA_SxM0AR(base)            (base + 0x1CUL)
#define DMA_SxFCR(base)             (base + 0x24UL)

#define DMA_CR_EN_BIT               (0)
#define DMA_CR_DIR_BIT              (6)
#define DMA_CR_MINC_BIT             (10)
#define DMA_CR_PSIZE_BIT            (11)
#define DMA_CR_MSIZE_BIT            (13)
#define DMA_CR_CHSEL_BIT            (25)

#define DMA_FCR_FTH_BIT             (0)
#define DMA_FCR_DMDIS_BIT           (2)

char DMA_DATA_STREAM[DMA_MAX_STRLEN];

u32 dma_getStreamBase(u32 dma_channel, u32 stream)
{
    u32 base;
    if(dma_channel == DMA1)
        base = DMA1_BASE;
    else if (dma_channel == DMA2)
        base = DMA2_BASE;
    else if (dma_channel == DMA2D)
        base = DMA2D_BASE;
    else
        return 0;
    return (base + 0x18 * stream);
}

void dma1_init(void)
{
//    u32 base1;
    u32 base3;
    /* 1. enable the peripheral clock for the dma
     *    AHB1 Bus, RCC_AHB1Bus enable
     * */
    io_writeMask((void *)RCC_AHB1ENR, 1 << RCC_DMA1_EN_PIN,
                 MASK(1) << RCC_DMA1_EN_PIN);

    /* 2. identify the stream which is suitable for your peripheral
     *    - In STM32F746
     *      USART3_TX: Stream 3, Channel 4
     *      USART3_RX: Stream 1, Channel 4
     *    - Notice that, the register address = address + 0x18 * stream number
     * */
//    base1 = dma_getStreamBase(DMA1, 1);
    base3 = dma_getStreamBase(DMA1, 3);
    io_writeMask((void *) DMA_SxCR(base3), 4 << DMA_CR_CHSEL_BIT,
                 MASK(3) << DMA_CR_CHSEL_BIT);

    /* 3. Identify the channel number on which uart3 peripheral serial */

    /* 4. Program the source address (memory) */
    io_write((void *) DMA_SxM0AR(base3), (u32) DMA_DATA_STREAM);

    /* 5. Program the destination address (peripheral) */
    io_write((void *) DMA_SxPAR(base3), (u32) USART_TDR(USART3_BASE));

    /* 6. Program the number of data items to send */
    io_write((void *) DMA_SxNDTR(base3), DMA_MAX_STRLEN);

    /* 7.   The direction of data transfer, m2p, p2m, m2m
     * 7.1. We config the direction of DMA, that is, memory to peripheral
     * */
    io_writeMask((void *)DMA_SxCR(base3), 1 << DMA_CR_DIR_BIT,
                 MASK(2) << DMA_CR_DIR_BIT);

    /* 8. Program the source and destination data width
     * 8.1. We need to config the memory data size for DMA
     * 8.2. We need to config the peripheral data size for DMA
     * 8.3. Enable the memory auto-increment
     * */
    io_writeMask((void *)DMA_SxCR(base3), 0 << DMA_CR_MSIZE_BIT,
                 MASK(2) << DMA_CR_MSIZE_BIT);
    io_writeMask((void *)DMA_SxCR(base3), 0 << DMA_CR_PSIZE_BIT,
                 MASK(2) << DMA_CR_PSIZE_BIT);
    io_writeMask((void *)DMA_SxCR(base3), 1 << DMA_CR_MINC_BIT,
                 MASK(1) << DMA_CR_MINC_BIT);


    /* 9. Direct mode or fifo mode */
    io_writeMask((void *)DMA_SxFCR(base3), 1 << DMA_FCR_DMDIS_BIT,
                 MASK(1) <<DMA_FCR_DMDIS_BIT);

    /* 10. Select the fifo threshold */
    io_writeMask((void *)DMA_SxFCR(base3), 3 << DMA_FCR_FTH_BIT,
                 MASK(2) <<DMA_FCR_FTH_BIT);

    /* 11. Enable the circular mode if required */

    /* 12. Single transfer or burst transfer */

    /* 13. Configure the stream priority */

    /* 14. Enable the stream */
    io_writeMask((void *)DMA_SxCR(base3), 1 << DMA_CR_EN_BIT,
                 MASK(1) << DMA_CR_EN_BIT);
}