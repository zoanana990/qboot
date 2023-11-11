#include <qubitas/io.h>

void io_write(register u32 addr, register u32 val) {
    /**
     * r is put the value to general register
     * Qo is let the input be memory address, arm specific
     */
    asm volatile("str %1, %0"
            : : "Qo" (*(__vo u32 *) addr), "r" (val));
}

u32 io_read(register u32 addr) {
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

void io_writeMask(register u32 addr, register u32 val, register u32 mask) {
    io_write(addr, (io_read(addr) & ~(mask)) | (val & mask));
}

u32 io_readBit(register u32 addr, register u32 bit) {
    return (io_read(addr) >> bit) & 1;
}

void io_writeBit(register u32 addr, register u32 bit) {
    io_writeMask(addr, 1 << bit, 1 << bit);
}