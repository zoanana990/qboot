#ifndef __RCC_H__
#define __RCC_H__

/* typedef */
enum {
    RCC_GPIOA = 0,
    RCC_GPIOB,
    RCC_GPIOC,
    RCC_GPIOD,
    RCC_GPIOE,
    RCC_GPIOF,
    RCC_GPIOG,
    RCC_GPIOH,
    RCC_GPIOI,
    RCC_GPIOJ,
    RCC_GPIOK,
    RCC_CRCRST = 12,
    RCC_DMA1 = 21,
    RCC_DMA2,
    RCC_DMA2D,
} RCC_AHB1_BIT;

enum {
    RCC_TIM2 = 0,
    RCC_TIM3,
    RCC_TIM4,
    RCC_TIM5,
    RCC_TIM6,
    RCC_TIM7,
    RCC_TIM12,
    RCC_TIM13,
    RCC_TIM14,
    RCC_WWDG = 11,
    RCC_SPI2 = 14,
    RCC_SPI3,

} RCC_APB1_BIT;

/* export functions */
void rcc_enApb1(int hw_no);
void rcc_enApb2(int hw_no);
void rcc_enAhb1(int hw_no);

#endif