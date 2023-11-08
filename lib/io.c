#include <qubitas/io.h>

void io_write(u32 addr, u32 val) {
    /**
     * r is put the value to general register
     * Qo is let the input be memory address, arm specific
     */
    asm volatile("str %1, %0"
            : : "Qo" (*(__vo u32 *) addr), "r" (val));
}

u32 io_read(u32 addr) {
    /**
     * r is put the value to general register
     * Qo is let the input be memory address, arm specific
     */
    u32 val;
    asm volatile("ldr %0, %1"
            : "=r" (val)
            : "Qo" (*(__vo u32 *) addr));
    return val;
}

void io_writeMask(u32 addr, u32 val, u32 mask) {
    io_write(addr, (io_read(addr) & ~(mask)) | (val & mask));
}

u32 io_readBit(u32 addr, u32 bit) {
    return (io_read(addr) >> bit) & 1;
}

void io_writeBit(u32 addr, u32 bit) {
    io_writeMask(addr, 1 << bit, 1 << bit);
}