#include <qubitas/string.h>
#include <qubitas/uart.h>
#include <qubitas/dma.h>
#include <qubitas/crc.h>
#include <qubitas/gpio.h>

extern char DMA_DATA_STREAM[DMA_MAX_STRLEN];

int main()
{
    usart_init();
    crc_init();
    button_init();
    dma1_init();
    dma_configIntr(DMA1, DMA_STREAM3);
    dma_enStreamX(DMA1, DMA_STREAM3);

    char str[] = "Qubitas Bootloader @Copyright 2023.10.10\r\n";
    strncpy(DMA_DATA_STREAM, str, 60);
//    usart_txData((u8 *)DMA_DATA_STREAM);

    while(1)
    {
        usart_rxData();
    }

    /* not expect to go here */
    return 0;
}