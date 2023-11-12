#include <qubitas/string.h>
#include <qubitas/type.h>
#include <qubitas/utils.h>

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

int strcmp(const char *s1, const char *s2) {
    unsigned char c1, c2;

    while (1) {
        c1 = *s1++;
        c2 = *s2++;
        if (c1 != c2)
            return c1 < c2 ? -1 : 1;
        if (!c1)
            break;
    }
    return 0;
}

char *strtok(register char *s, register const char *delim) {
    register char *spanp;
    register int c, sc;
    char *tok;
    static char *last;

    if (s == NULL && (s = last) == NULL)
        return (NULL);

    /*
     * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
     */
cont:
    /* get the first character in the string */
    c = *s++;
    for (spanp = (char *)delim; (sc = *spanp++) != 0;) {
        if (c == sc)
            goto cont;
    }

    if (c == 0) {		/* no non-delimiter characters */
        last = NULL;
        return (NULL);
    }
    tok = s - 1;

    /*
     * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
     * Note that delim must have one NUL; we stop if we see that, too.
     */
    for (;;) {
        c = *s++;
        spanp = (char *)delim;
        do {
            if ((sc = *spanp++) == c) {
                if (c == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                last = s;
                return (tok);
            }
        } while (sc != 0);
    }
}

u32 strtoul(const char *ptr_n, char **ptr_end, register int base) {

}