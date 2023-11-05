#ifndef __PRINTK_H__
#define __PRINTK_H__

#include <qubitas/type.h>

int printk(char *format, ...);
int sprintk(char *buffer, char *format, ...);
int snprintk(char *buffer, size_t count, char *format, ...);

#endif