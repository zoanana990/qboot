#ifndef __IO_H__
#define __IO_H__

#include <qubitas/type.h>

void io_write(u32 addr, u32 val);

u32 io_read(u32 addr);

void io_writeMask(u32 addr, u32 val, u32 mask);

u32 io_readBit(u32 addr, u32 bit);

void io_writeBit(u32 addr, u32 bit);

#endif