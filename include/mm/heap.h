#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <qubitas/type.h>

/* Configure heap size */
#define CONFIG_HEAP_SIZE               (0x20000)

/* Define the start address and size of each memory region that together comprise the total FreeRTOS heap space
 * */
struct heap_region {
    u8 *ptr_start_addrss;
    size_t size_in_bytes;
};

void mm_init(void);

void *malloc(size_t size);

void free(void *);

#endif