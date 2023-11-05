#ifndef __CRC_H__
#define __CRC_H__

#include <qubitas/utils.h>
#include <qubitas/type.h>

/* CRC initialization */
void crc_init(void);

/* compute CRC value */
u32 crc_compute(u32 input);

/* CRC reset */
void crc_reset(void);

#endif