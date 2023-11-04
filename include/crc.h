#ifndef __CRC_H__
#define __CRC_H__

#include <utils.h>

/* CRC initialization */
void crc_init(void);

/* compute CRC value */
u32 crc_compute(u32 input);

/* CRC reset */
void crc_reset(void);

#endif