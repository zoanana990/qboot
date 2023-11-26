#include <qubitas/string.h>
#include <qubitas/type.h>
#include <qubitas/utils.h>
#include <qubitas/printk.h>

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
    /* used to check the beginning of delimiter */
    register char *spanp;

    /* c: current character, sc: split character */
    register int c, sc;

    /* token */
    char *tok;

    /* last beginning of position
     * Here we need to check the last position, thus we use global variable
     * */
    static char *last;

    /* check the string is NULL and the last position is NULL, then return */
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
            /* if it is delimiter, continue next character */
            goto cont;
    }

    /* no non-delimiter characters, set the last to NULL */
    if (c == 0) {
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
            /* check the character is delimiter */
            if ((sc = *spanp++) == c) {
                if (c == 0)
                    /* if the character is NULL, set the string is NULL */
                    s = NULL;
                else
                    /* or use the delimiter to NULL */
                    s[-1] = 0;
                /* check the next operation position */
                last = s;

                /* return the token position */
                return (tok);
            }
        } while (sc != 0);
    }
}

/**
 * `strtoul`: convert a string to u32
 * */
u32 strtoul(const char *ptr_n, char **ptr_end, register int base) {
    register const s8 *s = ptr_n;
    register u32 result;
    register s8 c;

    /* check the string has space */
    do {
        c = *s++;
    } while(IS_SPACE(c));

    /* check the base is hex, decimal, binary etc. */
    if((base == 0 || base == 16) && c == '0' && (*s == 'x' || *s == 'X')) {
        c = s[1];
        s += 2;
        base = 16;
    }

    if(base == 0)
        base = 10;

    /* add the number according to the base */
    for(result = 0;; c = *s++) {
        if(IS_DIGIT(c))
            c -= '0';
        else if (IS_ALPHA(c))
            /* get 'a' then add 10
             * Note: c - (a - 10) = c - a + 10
             * */
            c -= IS_UPPER(c) ? 'A' - 10 : 'a' - 10;
        else {
            break;
        }

        if(c >= base){
            break;
        }

        result *= base;
        result += c;
    }

    return result;
}