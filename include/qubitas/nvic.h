#ifndef __NVIC_H__
#define __NVIC_H__

#include <qubitas/type.h>


void nvic_enIrq(u8 irq_number);

void nvic_disIrq(u8 irq_number);

#endif