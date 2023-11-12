#ifndef __STRING_H__
#define __STRING_H__

#include <qubitas/type.h>

char *strncpy(char *dst, const char *src, register size_t n);

size_t strlen(const char *str);

int	strcmp (const char *, const char *);

char *strtok(register char *s, register const char *delim);

u32 strtoul(const char *ptr_n, char **ptr_end, register int base);

#endif