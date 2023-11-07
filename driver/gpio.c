#include <qubitas/gpio.h>
#include <qubitas/io.h>
#include <qubitas/utils.h>
#include <qubitas/nvic.h>
#include <qubitas/exti.h>

#define RCC_BASE                (0x40023800UL)
#define RCC_AHB1ENR(base)       (base + 0x30UL)
#define RCC_APB2ENR(base)       (base + 0x44UL)

#define GPIO_BASE               (0x40020000UL)

#define GPIO_MODE(base)         (base + 0x00UL)
#define GPIO_PUPD(base)         (base + 0x0CUL)
#define GPIO_AFRL(base)         (base + 0x20UL)
#define GPIO_AFRH(base)         (base + 0x24UL)

#define EXTI_BASE               (0x040013C00UL)
#define EXTI_IMR(base)          (base + 0x00UL)
#define EXTI_FTSR(base)         (base + 0x0CUL)

#define SYSCFG_BASE             (0x40013800UL)
#define SYSCFG_EXTICR4(base)    (base + 0x14UL)

#define BUTTON_PIN              (13)
#define BUTTON_EXTI             (40)

static u32 gpio_getBase(int port) {
    return (GPIO_BASE + port * 0x400);
}

void gpio_init(int port) {
    /* All GPIO port are connecting to the AHB1 Bus */
    u32 base = RCC_BASE;
    u32 data = io_read(RCC_AHB1ENR(base));
    data |= (1 << port);
    io_write(RCC_AHB1ENR(base), data);
}

void gpio_setMode(int port, int pin, int mode) {
    u32 base = gpio_getBase(port);
    u32 val = mode << (pin * 2);
    u32 mask = MASK(2) << (pin * 2);

    io_writeMask(GPIO_MODE(base), val, mask);
}

s32 gpio_getMode(int port, int pin) {
    u32 base = gpio_getBase(port);
    u32 data = io_read(GPIO_MODE(base));

    return (data >> (pin * 2) & MASK(2));
}

void gpio_setPuPd(int port, int pin, int pupd) {
    u32 base = gpio_getBase(port);
    u32 val = pupd << (2 * pin);
    u32 mask = MASK(2) << (2 * pin);
    io_writeMask(GPIO_PUPD(base), val, mask);
}

void gpio_setAltFunc(int port, int pin, int func) {
    u32 base = gpio_getBase(port);
    u32 addr = pin >= 8 ? GPIO_AFRH(base) : GPIO_AFRL(base);
    u32 idx = pin >= 8 ? (pin - 8) : pin;
    u32 mask = MASK(4) << (idx * 4);
    u32 val = func << (idx * 4);

    io_writeMask(addr, val, mask);
}

void button_init(void) {
    u32 base = EXTI_BASE;

    /* In stm32F746, button is connect on GPIO port C, pin 13 */
    /* 1. Enable the clock for the GPIOC peripheral */
    gpio_init(GPIOC);
    syscfg_init();

    /* 2. Keep the gpio pin in input mode */
    gpio_setMode(GPIOC, BUTTON_PIN, GPIO_INPUT);

    /* 3. Enable the interrupt over that gpio pin */
    io_write(EXTI_IMR(base), 1 << BUTTON_PIN);

    /* 4. Configure SYSCFG CR4 register */
    io_write(SYSCFG_EXTICR4(SYSCFG_BASE), 0x2 << 4);

    /* 5. Configure the edge detection on the gpio pin */
    io_write(EXTI_FTSR(base), 1 << BUTTON_PIN);

    /* 6. Enable the IRQ related to that gpio pin in NVIC of the processor 
     *    The irq number is 40
     */
    nvic_enIrq(BUTTON_EXTI);
}