#include <qubitas/printk.h>
#include <qubitas/uart.h>
#include <qubitas/utils.h>
#include <qubitas/string.h>

#define IS_DIGIT(d)             ((d) >= '0' && (d) <= '9')
#define C_TO_D(c)               ((c) - '0')
#define MAXBUF                  (sizeof(int) * 8)

#define PRINT_TRUNCATES      FALSE

typedef void (*putc)(char);

/*************************************************************************
 * _vsnprintf
 *
 * What do we support ?
 * %d:      Decimal conversion
 * %u:      Unsigned conversion
 * %x:      Hexadecimal conversion
 * %c:      Character
 * %s:      String
 * %m.n	    field width, precision
 * %-m.n    left adjustment
 *
 *************************************************************************/
void _printNum(register u32 u, register s32 base, putc out) {
    char buf[MAXBUF];
    register char *p = &buf[MAXBUF - 1];
    static char digits[] = "0123456789abcdef";

    do {
        *p-- = digits[u % base];
        u /= base;
    } while (u != 0);

    while (++p != &buf[MAXBUF])
        out(*p);
}

static void _do_print(register const char *fmt, va_list *argp, putc out) {
    register char c;
    s32 length;
    s32 plus_sign;
    s32 sign_char;
    s32 precision;

    /* number base, like base 10, base 16 etc. */
    s32 base;

    /* padding character */
    s8 padc;

    /* left adjustment */
    bool ladjust;

    /* alternative format */
    bool altfmt;

    /* need to truncate ? */
    bool truncate;

    u32 u;
    s32 n;
    s32 capitals;

    while ((c = *fmt) != '\0') {
        /* Normal Case, just print the string */
        if (c != '%') {
            out(c);
            fmt++;
            continue;
        }

        fmt++;
        length = 0;
        padc = ' ';
        plus_sign = 0;
        sign_char = 0;
        ladjust = FALSE;
        altfmt = FALSE;
        precision = -1;

        while (TRUE) {
            c = *fmt;
            if (c == '#')
                /* Alternate format */
                altfmt = TRUE;
            else if (c == '-')
                ladjust = TRUE;
            else if (c == '+')
                plus_sign = '+';
            else if (c == ' ') {
                if (plus_sign == 0)
                    plus_sign = ' ';
            } else
                break;
            fmt++;
        }

        if (c == '0') {
            padc = '0';
            c = *++fmt;
        }

        if (IS_DIGIT(c)) {
            while (IS_DIGIT(c)) {
                /* Calculate the padding length */
                length = 10 * length + C_TO_D(c);
                c = *++fmt;
            }
        } else if (c == '*') {
            /* Need to parse the argument to get the precision */
            length = va_arg(*argp, int);
            c = *++fmt;
            if (length < 0) {
                ladjust = !ladjust;
                length = -length;
            }
        }

        if (c == '.') {
            c = *++fmt;
            if (IS_DIGIT(c)) {
                precision = 0;
                while (IS_DIGIT(c)) {
                    precision = 10 * precision + C_TO_D(c);
                    c = *++fmt;
                }
            } else if (c == '*') {
                precision = va_arg(*argp, int);
                c = *++fmt;
            }
        }

        /* for using llu, lu etc. */
        if (c == 'l')
            c = *++fmt;

        truncate = FALSE;
        capitals = 0;

        switch (c) {
            case 'p':
                altfmt = TRUE;
            case 'x':
                truncate = PRINT_TRUNCATES;
                base = 16;
                goto print_unsigned;

            case 'u':
                truncate = PRINT_TRUNCATES;
                base = 10;
                goto print_unsigned;

            case 'd':
                truncate = PRINT_TRUNCATES;
                base = 10;
                goto print_signed;

            case 'c':
                /* put char */
                c = va_arg(*argp, int);
                out(c);
                break;

            case 's': {
                register char *p;
                register char *p2;

                if (precision == -1)
                    precision = INT_MAX;

                p = va_arg(*argp, char *);

                if (p == (char *) 0)
                    p = "";

                if (length > 0 && !ladjust) {
                    n = 0;
                    p2 = p;

                    for (; *p != '\0' && n < precision; p++)
                        n++;

                    p = p2;

                    while (n < length) {
                        out(' ');
                        n++;
                    }
                }

                n = 0;

                while (*p != '\0') {
                    if (++n > precision || (length > 0 && n > length))
                        break;

                    out(*p++);
                }

                if (n < length && ladjust) {
                    while (n < length) {
                        out(' ');
                        n++;
                    }
                }

                break;
            }

            print_signed:
                n = va_arg(*argp, s32);
                if (n >= 0) {
                    u = n;
                    sign_char = plus_sign;
                } else {
                    u = -n;
                    sign_char = '-';
                }
                goto print_num;

            print_unsigned:
                u = va_arg(*argp, u32);
                goto print_num;
            print_num:
            {
                char buf[MAXBUF];    /* build number here */
                register char *p = &buf[MAXBUF - 1];
                static char digits[] = "0123456789abcdef0123456789ABCDEF";
                char *prefix = 0;

                if (u != 0 && altfmt)
                    if (base == 16)
                        prefix = "0x";

                do {
                    /* Print in the correct case */
                    *p-- = digits[(u % base) + capitals];
                    u /= base;
                } while (u != 0);

                length -= (&buf[MAXBUF - 1] - p);
                if (sign_char)
                    length--;
                if (prefix)
                    length -= strlen((const char *) prefix);

                if (padc == ' ' && !ladjust) {
                    /* blank padding goes before prefix */
                    while (--length >= 0)
                        out(' ');
                }
                if (sign_char)
                    out(sign_char);
                if (prefix)
                    while (*prefix)
                        out(*prefix++);
                if (padc == '0') {
                    /* zero padding goes after sign and prefix */
                    while (--length >= 0)
                        out('0');
                }
                while (++p != &buf[MAXBUF])
                    out(*p);

                if (ladjust) {
                    while (--length >= 0)
                        out(' ');
                }
                break;
            };

            case '\0':
                fmt--;
                break;

            default:
                out(c);
        }
        fmt++;
    }
}

/**************************************
 * EXPORT API
 **************************************/

int printk(char *format, ...) {
    va_list va;
    va_start(va, format);
    _do_print(format, &va, put_c);
    return 0;
}

int sprintk(char *buffer, char *format, ...) {
    return 0;
}