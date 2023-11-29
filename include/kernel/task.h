#ifndef __TASK_H__
#define __TASK_H__

#include <qubitas/type.h>

#define MAX_TASKS               (2)
#define STACK_SIZE              (256)

#define HANDLER_MSP             (0xFFFFFFF1UL)
#define THREAD_MSP              (0xFFFFFFF9UL)
#define THREAD_PSP              (0xFFFFFFFDUL)

#define SCS_BASE                (u32) (0xE000E000)
#define SCB_BASE                (SCS_BASE + 0x0D00)
#define SCB_ICSR                (u32 __vo) (SCB_BASE + 0x004)
#define SCB_ICSR_PENDSVSET_BIT  (28)

/* bit[24] is thumb bit */
#define DUMMY_XPSR                      (0x01000000UL)

typedef void (*task_handler_t)(void *);

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

void task_init();
errno task_create(task_handler_t task_handler, void *ptr_user_data);
void task_start();
void task_kill(int task_id);

#endif