#ifndef __DMA_H__
#define __DMA_H__

#define DMA_MAX_STRLEN              (128UL)

enum {
    DMA1 = 1,
    DMA2,
    DMA2D
};

/* DMA1 init */
void dma1_init(void);

#endif