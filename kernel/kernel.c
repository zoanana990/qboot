#include <qubitas/type.h>
#include <kernel/kernel.h>
#include <qubitas/io.h>

void disable_systick_timer(void) {
    io_write(SYST_CSR, 0);
}

void enable_systick_timer(void) {
    /* set the systick control and status value
     * bit[0]: Systick counter enable
     * bit[1]: Systick interrupt enable
     * bit[2]: indicate the systick clock source
     * */
    io_write(SYST_CSR, (1 << SYSTICK_CSR_EN_BIT) |
                       (1 << SYSTICK_CSR_INTR_EN_BIT) |
                       (1 << SYSTICK_CSR_CLK_SRC_BIT));
}

void init_systick_timer(u32 tick_hz) {

    u32 count = (SYSTICK_TIME_CLOCK / tick_hz) - 1;

    /* set the systick reload value */
    io_write(SYST_RVR, 0);
    io_write(SYST_RVR, count);

    enable_systick_timer();
}

void syscall() {
    __asm__ __vo("SVC #0");
}