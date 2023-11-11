#include <mm/mm.h>
#include <qubitas/type.h>
#include <qubitas/utils.h>



void *memset(void *str, register int c, register size_t n) {

    /* Here, we should not use the for-loop method to fill the bytes
     * one on one. Instead, we should consider the sizeof(unsigned long) to
     * reduce the time complexity from O(n) to O(n/4)
     * */
    return NULL;
}