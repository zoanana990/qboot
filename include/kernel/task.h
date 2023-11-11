#ifndef __TASK_H__
#define __TASK_H__

#include <qubitas/type.h>

typedef void (*task_handler_t)(void);

struct task {
    /* process stack pointer value */
    u32 psp_value;
    u32 block_count;
    u32 current_state;
    task_handler_t task_handler;
};

enum {
    TASK_STATE_READY = 0,
    TASK_STATE_BLOCK,
    TASK_STATE_RUNNING,
    TASK_STATE_PENDING,
};

void task_create(void);

#endif