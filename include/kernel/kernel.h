#ifndef __KERNEL_H__
#define __KERNEL_H__

#define SYST_CSR                        (0xE000E010UL)
#define SYST_RVR                        (0xE000E014UL)

#define SYSTICK_CSR_EN_BIT              (0)
#define SYSTICK_CSR_INTR_EN_BIT         (1)
#define SYSTICK_CSR_CLK_SRC_BIT         (2)

#define HSI_CLOCK                       (16000000UL)
#define SYSTICK_TIME_CLOCK              HSI_CLOCK
#define TICK_HZ                         (1000U)

void syscall();

void disable_systick_timer(void);
void enable_systick_timer(void);
void init_systick_timer(u32 tick_hz);

#endif
