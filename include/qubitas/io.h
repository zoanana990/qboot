#ifndef __IO_H__
#define __IO_H__

#include <qubitas/type.h>

void io_write(register u32 addr, register u32 val);

u32 io_read(register u32 addr);

void io_writeMask(register u32 addr, register u32 val, register u32 mask);

u32 io_readBit(register u32 addr, register u32 bit);

void io_writeBit(register u32 addr, register u32 bit);

/* printf */

/* scanf */

#endif