#include <crc.h>

/* AHB peripheral clock enable register */
#define RCC_AHB1ENR *(__vo u32 *) 0x40023830U

/* CRC data register */
#define CRC_DR *(__vo u32 *) 0x40023000U
/* CRC control register */
#define CRC_CR *(__vo u32 *) 0x40023008U
/* initial CRC value */
#define CRC_INIT *(__vo u32 *) 0x40023010U

/* CRC hardware reset */
void crc_reset(void)
{
    CRC_CR |= 1 << 0;
}

/* CRC initialization */
void crc_init(void)
{
    /* CRC RCC enable */
    RCC_AHB1ENR |= 1 << 6;
    /* reset CRC */
    crc_reset();
}

/* compute CRC value */
u32 crc_compute(u32 input)
{
    CRC_DR = input;
    return CRC_DR;
}