#ifndef __UTIL_H__
#define __UTIL_H__

#define NULL                    ((void *) 0)
#define TRUE                    (1)
#define FALSE                   (0)
#define MASK(bit)               (~(~0U << (bit)))

#define INT_MAX                 (0x7fffffff)
#define INT_MIN                 (0x80000000)

#define BYTE_ALIGNMENT          (0x008)
#define BYTE_ALIGNMENT_MASK     (0x007)

#define WORD_ALIGNMENT          (0x020)
#define WORD_ALIGNMENT_MASK     (0x01f)

#define WORD_MASK               (MASK(3))
#define WORD_SIZE               sizeof(u32)

#define likely(x)    __builtin_expect(!!(x), 1)
#define unlikely(x)  __builtin_expect(!!(x), 0)

#define ASSERT(__EXPR__)                                                \
    do {                                                                \
       if((__EXPR__)) {                                                 \
           printk("expression: %#x\r\n", (__EXPR__));                   \
           while(1);                                                    \
       }                                                                \
    } while(0);

int atoi(const char *nptr);

#endif