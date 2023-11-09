
#include <qubitas/utils.h>
#include <qubitas/uart.h>
#include <qubitas/printk.h>
#include <qubitas/io.h>

/* some stack memory calculations */
#define SIZE_TASK_STACK                 (1024U)
#define SIZE_SCHED_STACK                (1024U)

/* defined in ARM m7 technical manual */
#define SRAM_START                      (0x20000000U)
#define SRAM_SIZE                       ((128) * (1024))
#define SRAM_END                        (SRAM_START + SRAM_SIZE)

#define T1_STACK_START                  (SRAM_END)
#define T2_STACK_START                  (T1_STACK_START - SIZE_TASK_STACK)
#define T3_STACK_START                  (T2_STACK_START - SIZE_TASK_STACK)
#define T4_STACK_START                  (T3_STACK_START - SIZE_TASK_STACK)
#define SCHED_STACK_START               (T4_STACK_START - SIZE_TASK_STACK)

/* system handler control and state register */
#define SHCSR                           (0xE000ED24UL)

#define SHCSR_MEMFAULT_BIT              (16)
#define SHCSR_BUSFAULT_BIT              (17)
#define SHCSR_USGFAULT_BIT              (18)

#define SYST_CSR                        (0xE000E010UL)
#define SYST_RVR                        (0xE000E014UL)
#define SYST_CVR                        (0xE000E018UL)
#define SYST_CALIB                      (0xE000E01CUL)

#define SYSTICK_CSR_EN_BIT              (0)
#define SYSTICK_CSR_INTR_EN_BIT         (1)
#define SYSTICK_CSR_CLK_SRC_BIT         (2)

#define HSI_CLOCK                       (16000000UL)
#define SYSTICK_TIME_CLOCK              HSI_CLOCK
#define TICK_HZ                         (1000U)

#define MAX_TASKS                       (4)

/* bit[24] is thumb bit */
#define DUMMY_XPSR                      (0x01000000UL)



/***************************************
 * Global Variable
 ***************************************/
u32 psp_of_tasks[MAX_TASKS] = {
        T1_STACK_START,
        T2_STACK_START,
        T3_STACK_START,
        T4_STACK_START
};

u32 task_handlers[MAX_TASKS];

int current_task = 0;

void task1_handler(void);

void task2_handler(void);

void task3_handler(void);

void task4_handler(void);

void init_systick_timer(u32 tick_hz);

void init_task_stack(void);

void __attribute__((naked)) init_scheduler_stack(u32 sched_top_of_stack);

void switch_sp_to_psp();

void enable_processor_faults();

int main() {

    usart_init();

    pr_info("Start kernel... \n");

    enable_processor_faults();

    init_scheduler_stack(SCHED_STACK_START);

    task_handlers[0] = (u32) task1_handler;
    task_handlers[1] = (u32) task2_handler;
    task_handlers[2] = (u32) task3_handler;
    task_handlers[3] = (u32) task4_handler;

    init_task_stack();
    init_systick_timer(TICK_HZ);

    switch_sp_to_psp();

    task1_handler();

    while (1);
}

void enable_processor_faults() {
    /* Here we need to open the system handler control and state register */
    io_writeBit(SHCSR, SHCSR_MEMFAULT_BIT);
    io_writeBit(SHCSR, SHCSR_BUSFAULT_BIT);
    io_writeBit(SHCSR, SHCSR_USGFAULT_BIT);
}

u32 get_psp_value(void) {

    return psp_of_tasks[current_task];
}

void __attribute__((naked)) switch_sp_to_psp() {
    /* 1.   initialize the PSP with TASK1 stack start
     * 1.1. get the value of psp of current task
     * */

    /* BL instruction: branch with link(immediate) calls a subroutine at a PC-relative address
     * format: BL <label>
     * The assembler calculates the required of the offset from the PC value of the BL instruction to this label
     * then set the immediate32 offset.
     *
     * we should not use B here, because we need to go back to this function, therefore, before we go to the function
     * we need to save our {LR} first
     * */
    __asm__ __vo("PUSH {LR}"); /* preserve LR which connects back to main */
    __asm__ __vo("BL get_psp_value");

    /* MSR instruction: move to special register from the value of a general-purpose register to
     * the selected special purpose register
     * */
    __asm__ __vo("MSR PSP, R0"); /* Initialize PSP */

    __asm__ __vo("POP {LR}");    /* Pop back LR value */

    /* 2. Change SP to PSP using control register */

    /* We need to use special-purpose control register bit[1]
     * SPSEL (Stack pointer selection)
     *  bit[1] set 1 as using sp_process, that is (psp)
     *
     * In assembly, CONTROL means (The special-purpose CONTROL register)
     * */
    __asm__ __vo("MOV R0, #0x02");
    __asm__ __vo("MSR CONTROL, R0");

    /* return */
    __asm__ __vo("BX LR");
}

void __attribute__((naked)) init_scheduler_stack(u32 sched_top_of_stack) {


    __asm__ __vo("MSR MSP, %0":: "r"(sched_top_of_stack):);

    /**
     * BX instruction: Branch and exchange
     * BX can also be used for exception return
     * BX LR --> jump to the link register
     * */
    __asm__ __vo("BX LR");
}

void task1_handler(void) {
    while (1) {
        printk(DEFAULT "task #1 \r\n");
    }
}

void task2_handler(void) {
    while (1) {
        printk(DEFAULT "task #2 \r\n");
    }
}

void task3_handler(void) {
    while (1) {
        printk(DEFAULT "task #3 \r\n");
    }
}

void task4_handler(void) {
    while (1) {
        printk(DEFAULT "task #4 \r\n");
    }
}

void init_task_stack(void) {
    u32 *ptr_psp;
    for (int i = 0; i < MAX_TASKS; i++) {
        ptr_psp = (u32 *) psp_of_tasks[i];

        /* 0x00100000 */
        ptr_psp--;
        *ptr_psp = DUMMY_XPSR;

        /* program counter */
        ptr_psp--;
        *ptr_psp = task_handlers[i];

        /* Link register, return back to the process stack pointer */
        ptr_psp--;
        *ptr_psp = 0xFFFFFFFD;

        /* Initialize General purpose register */
        for(int j = 0; j < 13; j++)
        {
            ptr_psp--;
            *ptr_psp = 0;
        }

        /* store the process stack pointer */
        psp_of_tasks[i] = (u32) ptr_psp;
    }
}

void init_systick_timer(u32 tick_hz) {

    u32 count = (SYSTICK_TIME_CLOCK / tick_hz) - 1;

    /* set the systick reload value */
    io_write(SYST_RVR, 0);
    io_write(SYST_RVR, count);

    /* set the systick control and status value
     * bit[0]: Systick counter enable
     * bit[1]: Systick interrupt enable
     * bit[2]: indicate the systick clock source
     * */
    io_write(SYST_CSR, (1 << SYSTICK_CSR_EN_BIT) |
                       (1 << SYSTICK_CSR_INTR_EN_BIT) |
                       (1 << SYSTICK_CSR_CLK_SRC_BIT));
}

/*------------------------------------------ Exception Function Handler */

void SysTick_Handler(void) {
    /*** Context Switch ***/

    /**
     * I. Save the context of current task
     *    1. Get the current running task's PSP value
     *    2. Using that PSP value store SF2 (R4 to R11)
     *    3. Save the current value of PSP
     * */
    __asm__ __vo()

    /**
     * II. Retrieve the context of next stack
     *    1. Decide next stack to run
     *    2. Get its past PSP value
     *    3. Using that PSP value retrieve SF2 (R4 to R11)
     *    4. Update the PSP and exit
     * */
}

void MemManage_Handler(void) {
    pr_excp("MemManage Handler... \r\n");
    while(1);
}

void BusFault_Handler(void) {
    pr_excp("BusFault Handler... \r\n");
    while(1);
}

void UsageFault_Handler(void) {
    pr_excp("UsageFault Handler... \r\n");
    while(1);
}