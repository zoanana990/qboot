#include <qubitas/string.h>
#include <qubitas/type.h>

char *strncpy(char *dst, const char *src, register size_t n) {
    if (n != 0) {

        /* Use register to accelerate the pointer access */
        register char *d = dst;
        register const char *s = src;

        do {
            if ((*d++ = *s++) == 0) {
                /* NULL pad the remaining n-1 bytes */
                while (--n != 0)
                    *d++ = 0;
                break;
            }
        } while (--n != 0);
    }
    return (dst);
}

size_t strlen(const char *str) {
    size_t length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}