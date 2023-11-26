/**
 * This file is covered STM32F746 timer and rcc modules
 * */

#include <qubitas/timer.h>
#include <qubitas/io.h>

#define RCC_BASE                    (0x40023800UL)

#define RCC_CR                      (RCC_BASE + 0x00UL)
#define RCC_CFGR                    (RCC_BASE + 0x08UL)
#define RCC_AHB1ENR                 (RCC_BASE + 0x30UL)
#define RCC_APB1ENR                 (RCC_BASE + 0x40UL)
#define RCC_APB1RSTR                (RCC_BASE + 0x20UL)

void rcc_configClk(int clk_sor) {

}

