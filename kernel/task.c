#include <kernel/task.h>
#include <qubitas/utils.h>

void idle_task(void *p) {
    while (likely(1)) {
        __asm__ __vo("NOP");
    }
}

/* create the task */
errno task_create() {
    return 0;
}

/* free the stack of task */
void task_fill() {

}

void task_start() {

}