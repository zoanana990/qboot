#ifndef __NVIC_H__
#define __NVIC_H__

#include <qubitas/type.h>

#define ENABLE_GLOBAL_INTERRUPT
#define DISABLE_GLOBAL_INTERRUPT

#define MEMORY_BARRIER              __asm__ __vo ("" ::: "memory")

void nvic_enIrq(u8 irq_number);

void nvic_disIrq(u8 irq_number);

#endif