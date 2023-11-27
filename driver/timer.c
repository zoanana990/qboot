/**
 * This file is covered STM32F746 timer and rcc modules
 * */

#include <qubitas/timer.h>
#include <qubitas/io.h>
#include <qubitas/printk.h>

#define RCC_BASE                    (0x40023800UL)
#define RCC_APB1ENR                 (RCC_BASE + 0x40UL)

#define RCC_APB1ENR_TIM6EN          (4)
#define RCC_APB1ENR_TIM7EN          (5)


/* Basic timer */
#define TIM6_BASE                   (0x40001000UL)
#define TIM7_BASE                   (0x40001400UL)


u32 timer_getBase(int timer_no) {
    switch(timer_no) {
        case TIM6:
            return TIM6_BASE;
        case TIM7:
            return TIM7_BASE;
        default:
            break;
    }
    printk("timer_no not found\r\n");
    return 0;
}

void timer_init(int timer_no) {

//    u32 base = timer_getBase(timer_no);
//
//    /* basic timer, 16 bit */
//    if((timer_no == TIM6) || (timer_no == TIM7)) {
//        /* activate rcc in APB bus */
//    }
}
