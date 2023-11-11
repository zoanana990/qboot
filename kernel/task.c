#include <kernel/task.h>
#include <mm/heap.h>
#include <mm/mm.h>
#include <qubitas/type.h>
#include <qubitas/utils.h>

struct task *_task_create(void) {

    /* 1. allocate the struct task space */
    u32 *ptr_stack = malloc(sizeof(u32) * stack_size);
    struct task *ptr_task;

    /* 2. allocate the stack space */
    if (ptr_stack != NULL) {
        ptr_task = malloc(sizeof(struct task));
        if (ptr_task != NULL) {
            memset(ptr_task, 0x00, sizeof(struct task));
            ptr_task->ptr_stack = ptr_stack;
        } else {
            free(ptr_stack);
            ptr_stack = NULL;
        }
    } else {
        ptr_stack == NULL;
    }

    _task_initTask();

    return NULL;
}

errno task_create(void *) {

    errno res;
    struct task *ptr_task;

    /* 1. create a task, filled with the task control block */
    ptr_task = _task_create();

    if (ptr_task == NULL) {
        res = ERRNO_NOMEM;
    } else {
        /* 2. add the new task to the ready list */
        task_addToReadyList(NULL);
        res = ERRNO_OK;
    }

    return res;
}


errno task_addToReadyList(struct task *ptr_task) {
    /* TODO, add the task to the ready list */
    return ERRNO_OK;
}
