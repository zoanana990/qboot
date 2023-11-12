#include <mm/mm.h>
#include <qubitas/type.h>
#include <qubitas/utils.h>

//void *memset(void *dst0, register int c0, register size_t len) {
//
//    /***
//     * Here, we should not use the for-loop method to fill the bytes
//     * one on one. Instead, we should consider the sizeof(unsigned long) to
//     * reduce the time complexity from O(n) to O(n/4) for bigger array
//     * */
//    register size_t l = len / sizeof(u32);
//    register u32 c;
//    register u8 *dst;
//
//    dst = dst0;
//
//    /**
//     * If not enough words, just fill bytes.
//     * */
//    if (len < 3 * WORD_SIZE) {
//        while (len != 0) {
//            *dst++ = c0;
//            --len;
//        }
//        return (dst0);
//    }
//
//    /**
//     * Enough bytes, fill the word
//     * */
//    if ((c = (u8) c0) != 0) {
//        c = (c << 8) | c;	   /* u32 is 16 bits. */
//        c = (c << 16) | c;	   /* u32 is 32 bits. */
//    }
//
//    /* Align destination by filling in bytes. */
//    if ((l = (int)dst & WORD_MASK) != 0) {
//        l = sizeof(u32) - l;
//        len -= l;
//        do {
//            *dst++ = c0;
//        } while (--l != 0);
//    }
//
//    /* Fill words.  Length was >= 2*words so we know t >= 1 here. */
//    l = len / WORD_SIZE;
//    do {
//        *(u32 *)dst = c;
//        dst += WORD_SIZE;
//    } while (--l != 0);
//
//    /* Mop up trailing bytes, if any. */
//    l = len & WORD_MASK;
//    if (l != 0)
//        do {
//            *dst++ = c0;
//        } while (--l != 0);
//    return (dst0);
//}
void *memset(void *dst0, register int c0, register size_t len) {
    register u8 *dst = dst0;
    while (len != 0) {
        *dst++ = c0;
        --len;
    }
    return (dst0);
}