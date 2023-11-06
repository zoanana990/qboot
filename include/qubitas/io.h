#ifndef __IO_H__
#define __IO_H__

#include <qubitas/type.h>

void io_write(void *addr, u32 val);
u32 io_read(void *addr);
void io_writeMask(void *addr, u32 val, u32 mask);

#endif