#ifndef __DMA_H__
#define __DMA_H__

#include <qubitas/type.h>

#define DMA_MAX_STRLEN              (128UL)

enum {
    DMA1 = 1,
    DMA2,
    DMA2D
};

enum {
    DMA_STREAM0 = 0,
    DMA_STREAM1,
    DMA_STREAM2,
    DMA_STREAM3,
    DMA_STREAM4,
    DMA_STREAM5,
    DMA_STREAM6,
    DMA_STREAM7,
};

u32 dma_getStreamBase(u32 dma_channel, u32 stream);

/* DMA stream init */
void dma_enStreamX(u32 dma_channel, u32 stream);

void dma_configIntr(u32 dma_channel, u32 stream);

/* DMA1 init */
void dma_initUart3Tx(void);

void dma_initUart3Rx(void);

#endif