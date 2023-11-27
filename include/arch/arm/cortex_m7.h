#ifndef __CORTEX_M7__
#define __CORTEX_M7__

#include <qubitas/type.h>

#if 0
#define INTERRUPT_ENABLE()                  \
    do {                                    \
        __asm volatile("MOV R0, #0x1");     \
        __asm volatile("MOV PRIMASK, R0");  \
    } while(0)

#define INTERRUPT_DISABLE()                 \
    do {                                    \
        __asm volatile("MOV R0, #0x0");     \
        __asm volatile("MOV PRIMASK, R0");  \
    } while(0)
#else
#define INTERRUPT_ENABLE()  __asm volatile("cpsie i")
#define INTERRUPT_DISABLE()  __asm volatile("cpsid i")
#endif
#endif