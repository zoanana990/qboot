#include <kernel/task.h>
#include <qubitas/utils.h>
#include <qubitas/io.h>
#include <qubitas/printk.h>
#include <mm/heap.h>
#include <mm/mm.h>

struct task_struct {
    void *stack;
    void *origin_stack;
    u8 task_id;
    u8 in_use;
    u8 priority;
};

static struct task_struct tasks[MAX_TASKS];
static int last_task;
// static int current_task_count;

void task_init() {
    memset(tasks, 0x0, sizeof(struct task_struct) * MAX_TASKS);
}

void idle_task(void *p) {
    while (likely(1)) {
        __asm__ __vo("NOP");
    }
}

void thread_self_terminal()
{
    /* This will kill the stack.
        * For now, disable context switches to save ourselves.
        */
    asm volatile("cpsid i\n");
    task_kill(last_task);
    asm volatile("cpsie i\n");

    /* And now wait for death to kick in */
    while (1);
}

/* create the task */
errno task_create(task_handler_t task_handler, void *ptr_user_data) {

    int task_id = 0;
    u32 *ptr_stack;

    for(task_id = 0; task_id < MAX_TASKS; task_id++) 
        if(tasks[task_id].in_use == 0)
            break;

    if(task_id == MAX_TASKS)
        return -ERRNO_NOMEM;

    ptr_stack = kalloc(STACK_SIZE * sizeof(u32));
    if(ptr_stack == NULL)
        return -ERRNO_NOMEM;
    
    tasks[task_id].origin_stack = ptr_stack;
    ptr_stack += STACK_SIZE - 17;
    ptr_stack[8] = (u32) THREAD_PSP;
    ptr_stack[9] = (u32) ptr_user_data;
    ptr_stack[14] = (u32)&thread_self_terminal;
    ptr_stack[15] = (u32)task_handler;
    ptr_stack[16] = DUMMY_XPSR;

    tasks[task_id].stack = ptr_stack;
    tasks[task_id].in_use = 1;
    tasks[task_id].task_id = task_id;

    return 0;
}

/* free the stack of task */
void task_kill(int task_id) {
    tasks[task_id].in_use = 0;
    kfree(tasks[task_id].origin_stack);
}

__attribute__((naked)) void task_start() {
    last_task = 0;
    /* Reset APSR before context switch.
    * Make sure we have a _clean_ PSR for the task.
    */
    asm volatile("mov r0, #0\n"
                "msr APSR_nzcvq, r0\n");
    /* To bridge the variable in C and the register in ASM,
    * move the task's stack pointer address into r0.
    * http://www.ethernut.de/en/documents/arm-inline-asm.html
    */
    asm volatile("mov r0, %0\n" : : "r" (tasks[last_task].stack));
    asm volatile("msr psp, r0\n"
                "mov r0, #3\n"
                "msr control, r0\n"
                "isb\n");
    /* This is how we simulate stack handling that pendsv_handler
    * does. Thread_create sets 17 entries in stack, and the 9
    * entries we pop here will be pushed back in pendsv_handler
    * in the same order.
    *
    *
    *                      pop {r4-r11, lr}
    *                      ldr r0, [sp]
    *          stack
    *  offset -------
    *        |   16  | <- Reset value of PSR
    *         -------
    *        |   15  | <- Task entry
    *         -------
    *        |   14  | <- LR for task
    *         -------
    *        |  ...  |                             register
    *         -------                              -------
    *        |   9   | <- Task argument ---->     |   r0  |
    * psp after pop--<                             -------
    *        |   8   | <- EXC_RETURN    ---->     |   lr  |
    *         -------                              -------
    *        |   7   |                            |  r11  |
    *         -------                              -------
    *        |  ...  |                            |  ...  |
    *         -------                              -------
    *        |   0   |                            |   r4  |
    * psp ->  -------                              -------
    *
    * Instead of "pop {r0}", use "ldr r0, [sp]" to ensure consistent
    * with the way how PendSV saves _old_ context[1].
    */
    asm volatile("pop {r4-r11, lr}\n"
                "ldr r0, [sp]\n");
    /* Okay, we are ready to run first task, get address from
    * stack[15]. We just pop 9 register so #24 comes from
    * (15 - 9) * sizeof(entry of sp) = 6 * 4.
    */
    asm volatile("ldr pc, [sp, #24]\n");

    /* Never reach here */
    while(1);
}

/* Caution: Without naked attribute, GCC will normally push r7 which is used
 * for stack pointer. If so, after restoring the tasks' context, we will get
 * wrong stack pointer. PendSV handles calling convention itself.
 */
void __attribute__((naked)) PendSV_Handler()
{
    /* Save the old task's context[1]*/
    asm volatile("mrs   r0, psp\n"
                    "stmdb r0!, {r4-r11, lr}\n");
    /* To get the task pointer address from result r0 */
    asm volatile("mov   %0, r0\n" : "=r" (tasks[last_task].stack));

    /* Find a new task to run */
    while (1) {
        last_task++;
        if (last_task == MAX_TASKS)
            last_task = 0;
        if (tasks[last_task].in_use) {
            /* Move the task's stack pointer address into r0 */
            asm volatile("mov r0, %0\n" : : "r" (tasks[last_task].stack));
            /* Restore the new task's context and jump to the task */
            asm volatile("ldmia r0!, {r4-r11, lr}\n"
                            "msr psp, r0\n"
                            "bx lr\n");
        }
    }
}


void SysTick_Handler(void)
{
    // printk("in handler\r\n");
    io_writeBit(SCB_ICSR, SCB_ICSR_PENDSVSET_BIT);
    // printk("here\r\n");
}

void MemManage_Handler(void) {
    pr_excp("MemManage Handler... \r\n");
    while (1);
}

void BusFault_Handler(void) {
    pr_excp("BusFault Handler... \r\n");
    while (1);
}

void UsageFault_Handler(void) {
    pr_excp("UsageFault Handler... \r\n");
    while (1);
}