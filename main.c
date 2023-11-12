#include <qubitas/uart.h>
#include <qubitas/dma.h>
#include <qubitas/printk.h>
#include <mm/heap.h>

extern char DMA_TX_DATA_STREAM[DMA_MAX_STRLEN];

int main() {

    usart_init();
    printk("\r\n");
    pr_ok("Compile Time: %s\r\n", __DATE__);
    pr_ok("Kernel Start ... \r\n");
    pr_ok("Khienh Qubitas kernel MIT@Liecense\r\n");

    pr_ok("Start init heap\r\n");
    mm_init();
    pr_ok("Heap init successfully\r\n");

    printk("\r\n");
    printk("   ###    #     #  ######    #####   #######     #      #####\r\n");
    printk("  #   #   #     #  #     #     #        #        #     #     #\r\n");
    printk(" #     #  #     #  #     #     #        #       ###    #\r\n");
    printk(" #     #  #     #  ######      #        #       # #     #####\r\n");
    printk(" #     #  #     #  #     #     #        #      #####         #\r\n");
    printk("  #   #   #     #  #     #     #        #      #   #   #     #\r\n");
    printk("   ###     #####   ######    #####      #     ##   ##   #####\r\n");
    printk("     ###     AN OPEN SOURCE TUTORIAL OPERATING SYSTEM IN 2023\r\n");

    while (1);

    /* not expect to go here */
    return 0;
}