#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <qubitas/type.h>

/* Configure heap size */
#define CONFIG_HEAP_SIZE               (0x20000)

void *malloc(size_t size);
void free(void *)

#endif