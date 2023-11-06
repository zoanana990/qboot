#ifndef __EXTI_H__
#define __EXTI_H__

void clear_exti_pending_bit(int pin_number);
void syscfg_init(void);

#endif