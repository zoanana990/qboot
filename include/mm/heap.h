#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <qubitas/type.h>

void mm_init(void);

void *kalloc(size_t size);

void kfree(void *p);

#endif