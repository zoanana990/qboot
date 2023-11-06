#ifndef __GPIO_H__
#define __GPIO_H__

#include <qubitas/type.h>

enum {
    GPIOA = 0,
    GPIOB,
    GPIOC,
    GPIOD,
    GPIOE,
    GPIOF,
    GPIOG,
    GPIOH,
    GPIOI,
    GPIOJ,
    GPIOK,
};

enum {
    GPIO_INPUT = 0,
    GPIO_OUTPUT,
    GPIO_ALTERNATE,
    GPIO_ANALOG,
};

/* TODO: use a better method to store the function */
enum {
    GPIO_FUNC_SYS   = 0,

    GPIO_FUNC_TIM1  = 1,
    GPIO_FUNC_TIM2  = 1,
    
    GPIO_FUNC_TIM3  = 2,
    GPIO_FUNC_TIM4  = 2,
    GPIO_FUNC_TIM5  = 2,

    GPIO_FUNC_TIM8  = 3,
    GPIO_FUNC_TIM9  = 3,
    GPIO_FUNC_TIM10 = 3,
    GPIO_FUNC_TIM11 = 3,

    GPIO_FUNC_I2C   = 4,

    GPIO_FUNC_SPI   = 5,

    GPIO_FUNC_USART = 7,
};

void gpio_init(int port);
void gpio_setMode(int port, int pin, int mode);
s32 gpio_getMode(int port, int pin);
void gpio_setAltFunc(int port, int pin, int func);
void button_init(void);

#endif