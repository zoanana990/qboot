#include <app/app.h>
#include <kernel/task.h>
#include <kernel/kernel.h>
#include <qubitas/printk.h>
#include <qubitas/uart.h>
#include <qubitas/type.h>
#include <qubitas/io.h>

static void delay(volatile int count)
{
    count *= 50000;
    while (count--);
}

static void busy_loop(void *str)
{
    while (1) {
        printk("%s\r\n", str);
        delay(40);
    }
}

void task1(void *ptr_user_data) {
    busy_loop(ptr_user_data);
}

void task2(void *ptr_user_data) {
    busy_loop(ptr_user_data);
}

void cs_main(void) {

    pr_info("Sample code for context switch... \n");

    int rc;

    s8 *task1_name = "task1";
    s8 *task2_name = "task2";

    rc = task_create(task1, (void *)task1_name);
    if(rc) {
        pr_err("task 1 create fail, rc = %d\r\n", rc);
    }

    rc = task_create(task2, (void *)task2_name);
    if(rc) {
        pr_err("task 2 create fail, rc = %d\r\n", rc);
    }

    init_systick_timer(TICK_HZ);

    task_start();
}

