#ifndef __PRINTK_H__
#define __PRINTK_H__

#include <qubitas/type.h>

void put_c(char *ptr_tx_buffer);
int printk(char *format, ...);
int sprintk(char *buffer, char *format, ...);
int snprintk(char *buffer, size_t count, char *format, ...);

#endif