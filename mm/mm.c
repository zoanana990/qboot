#include <mm/mm.h>
#include <qubitas/type.h>

void *memset(void *dst0, register int c0, register size_t len) {
    register u8 *dst = dst0;
    while (len != 0) {
        *dst++ = c0;
        --len;
    }
    return (dst0);
}