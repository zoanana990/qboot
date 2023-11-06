#include <qubitas/nvic.h>
#include <qubitas/type.h>

/* Maxima irq number in stm32f746 */
#define IRQ_MAX_NUMBER      (97)

#define NVIC_ISER           ((__vo u32 *) 0xE000E100UL)
#define NVIC_ICER           ((__vo u32 *) 0xE000E180UL)

void nvic_enIrq(u8 irq_number)
{
    if(irq_number > IRQ_MAX_NUMBER)
        return;

    u8 offset = irq_number >> 5;
    u8 pos = irq_number & 31;
    NVIC_ISER[offset] |= 1 << pos;
}

void nvic_disIrq(u8 irq_number)
{

}