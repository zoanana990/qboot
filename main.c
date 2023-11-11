#include <qubitas/uart.h>
#include <qubitas/dma.h>
#include <qubitas/printk.h>
#include <mm/heap.h>

extern char DMA_TX_DATA_STREAM[DMA_MAX_STRLEN];


int main() {

    usart_init();
    pr_info("Start kernel... \r\n");
    pr_info("Qubitas Bootloader @Copyright\r\n");

    pr_info("Start init heap\r\n");
    mm_init();
    pr_info("Heap init successfully\r\n");

    while (1);

    /* not expect to go here */
    return 0;
}