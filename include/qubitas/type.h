#ifndef __TYPE_H__
#define __TYPE_H__

#define __vo                         volatile

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef long long s64;
typedef int s32;
typedef short s16;
typedef char s8;

typedef u32 size_t;
typedef s32 ssize_t;

typedef signed int bool;

typedef enum {
    ERRNO_OK = 0,
    ERRNO_NOMEM,
    ERRNO_NOT_SUP,
    ERRNO_OP_INVALID
} errno;

#ifdef __QUBITAS__
typedef __builtin_va_list va_list;
#define va_start(v, l)              __builtin_va_start(v, l)
#define va_end(v)                   __builtin_va_end(v)
#define va_arg(v, T)                __builtin_va_arg(v, T)
#define va_copy(d, s)               __builtin_va_copy(d, s)
#else
typedef char *                      va_list;
#define _AUPBND                     (sizeof (s32) - 1)
#define _ADNBND                     (sizeof (s32) - 1)
#define _bnd(X, bnd)                (((sizeof (X)) + (bnd)) & (~(bnd)))
#define va_arg(ap, T)               (*(T *)(((ap) += (_bnd (T, _AUPBND))) - (_bnd (T,_ADNBND))))
#define va_end(ap)                  (void) 0
#define va_start(ap, A)             (void) ((ap) = (((char *) &(A)) + (_bnd (A,_AUPBND))))
#endif

#endif