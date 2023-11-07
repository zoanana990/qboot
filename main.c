#include <qubitas/string.h>
#include <qubitas/uart.h>
#include <qubitas/dma.h>
#include <qubitas/crc.h>
#include <qubitas/gpio.h>
#include <qubitas/printk.h>

extern char DMA_TX_DATA_STREAM[DMA_MAX_STRLEN];

int main() {
    usart_init();
    crc_init();
    button_init();
    dma1_init();
    dma_configIntr(DMA1, DMA_STREAM3);
    dma_enStreamX(DMA1, DMA_STREAM3);

//    char str[] = "Qubitas Bootloader @Copyright 2023.10.10 \r\n";
//    strncpy(DMA_TX_DATA_STREAM, str, 60);
//    usart_txData((u8 *)DMA_TX_DATA_STREAM);
    printk("Kernel Compile time %s\r\n", __DATE__);
    printk("Start kernel... \r\n");
    printk("Qubitas Bootloader @Copyright %d\r\n", 6526);
    printk("[%10d] \r\n", 9499);
    printk("[%-10d] \r\n", 9499);
    printk("[%#x] \r\n", 9499);

    while (1) {
        usart_rxData();
    }

    /* not expect to go here */
    return 0;
}