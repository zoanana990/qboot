#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <qubitas/type.h>

void mm_init(void);

void *malloc(size_t size);

void free(void *p);

#endif