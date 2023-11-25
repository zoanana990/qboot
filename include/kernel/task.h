#ifndef __TASK_H__
#define __TASK_H__

#include <qubitas/type.h>

/* TODO: task suspend */
#define TASK_SUSPEND

/* TODO: task resume */
#define TASK_RESUME

typedef void (*task_handler_t)(void);

struct task {
    /* process stack pointer value */
    char name[16];
    u32 psp_value;
    u32 block_count;
    u32 current_state;
    u32 stack_size;
    u32 *ptr_stack;
    task_handler_t task_handler;
};

enum {
    TASK_STATE_READY = 0,
    TASK_STATE_BLOCK,
    TASK_STATE_RUNNING,
    TASK_STATE_PENDING,
};

errno task_create(void);


#endif