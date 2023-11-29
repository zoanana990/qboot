#include <qubitas/uart.h>
#include <qubitas/dma.h>
#include <qubitas/printk.h>
#include <mm/heap.h>
#include <app/app.h>
#include <kernel/kernel.h>
#include <kernel/task.h>

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

    pr_ok("Start init task\r\n");
    task_init();
    pr_ok("task init successfully\r\n");

    printk("\r\n");
    printk("   ###    #     #  ######    #####   #######     #      #####\r\n");
    printk("  #   #   #     #  #     #     #        #        #     #     #\r\n");
    printk(" #     #  #     #  #     #     #        #       ###    #\r\n");
    printk(" #     #  #     #  ######      #        #       # #     #####\r\n");
    printk(" #     #  #     #  #     #     #        #      #####         #\r\n");
    printk("  #   #   #     #  #     #     #        #      #   #   #     #\r\n");
    printk("   ###     #####   ######    #####      #     ##   ##   #####\r\n");
    printk("     ###     AN OPEN SOURCE TUTORIAL OPERATING SYSTEM IN 2023\r\n");
    printk("\r\n");
    pr_ok("Please press [enter] to get into the console \r\n");

    cs_main();
    // syscall();

    while (1);

    /* not expect to go here */
    return 0;
}