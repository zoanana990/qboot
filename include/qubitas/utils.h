#ifndef __UTIL_H__
#define __UTIL_H__

#define NULL                ((void *) 0)
#define TRUE                (1)
#define FALSE               (0)
#define MASK(bit)           (~(~0U << (bit)))

#define INT_MAX             (0x7fffffff)
#define INT_MIN             (0x80000000)

int atoi(const char *nptr);

#endif