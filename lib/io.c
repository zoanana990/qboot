#include <qubitas/io.h>

void io_write(void *addr, u32 val)
{
    /**
     * r is put the value to general register
     * Qo is let the input be memory address, arm specific
     */
    asm volatile("str %1, %0"
            : : "Qo" (*(__vo u32 *)addr), "r" (val));
}

u32 io_read(void *addr)
{
    /**
     * r is put the value to general register
     * Qo is let the input be memory address, arm specific
     */
    u32 val;
    asm volatile("ldr %0, %1"
                : "=r" (val)
                : "Qo" (*(__vo u32 *)addr));
    return val;
}

void io_writeMask(void *addr, u32 val, u32 mask)
{
    io_write(addr, (io_read(addr) & ~(mask)) | (val & mask));
}